% University of Surrey
%
% This file is part of TLD.
%

function [bb,conf,weights] = tldSaliency(opt)

global tld; % holds results and temporal variables

%%
%--SALIENCY-BEGIN--
%
global param;
param = {};
%---SALIENCY-END---
%%

% INITIALIZATION ----------------------------------------------------------

% select data source, camera/directory
opt.source = tldInitSource(opt.source);

% open figure for display of results
figure(2); set(2,'KeyPressFcn', @handleKey);
% by pressing any key, the process will exit8
finish = 0; function handleKey(~,~), finish = 1; end 

while 1
    % get initial bounding box, return 'empty' if bounding box is too small
    source = tldInitFirstFrame(tld,opt.source,opt.model.min_win); 
    % check size
    if ~isempty(source), opt.source = source; break; end 
end

% train initial detector and initialize the 'tld' structure
tld = tldInit(opt,[]); 
% initialize display
tld = tldDisplay(0,tld); 
    
%%
%--SALIENCY-BEGIN--

crtFrame = im2double(imread(source.files(1).name));
[param.resY, param.resX, param.nChannel] = size(crtFrame);
set_param;
crtBB = [max([1;1], tld.bb(1:2,1)); ...
         min([param.resX;param.resY], tld.bb(3:4,1))];

% some bsic data verification
if param.nChannel < 3
   disp('Color image expected. Process terminated.')
   return
elseif min(param.resY, param.resX) < 2^param.pyraScales
   disp('Minimise resolution should be 2^(scale of pyramid).');
   return
elseif length(tld.source.idx) < param.wSpan
   disp('Video too short.')
   return
end

% some other initializations
W = zeros(param.nEhcMaps, 6, param.wSpan);
flash = {};
flash.frm   = cell(param.flashL, 1);
flash.diffs = cell(param.flashL, 1);
flash.mask  = cell(param.flashL, 1);
flash.crtBB = cell(param.flashL, 1);

weights = zeros(param.nEhcMaps, 6, param.nFrames);
weightsIdx = 1;

writerObj = VideoWriter(strcat('result_',datestr(clock,'HHMMSS'),'.avi'));
set(writerObj, 'FrameRate', param.fps);
open(writerObj);

fprintf('\nframe \tmax ROI \tmean diff \n');
fprintf('001');

% prepare pyras and diff maps
pyrasBef = make_pyras(crtFrame);
diffs = make_diffs(pyrasBef);

% make mask
SBef = simple_n(enlarge(get_salimap(pyrasBef)));
mask = simple_n(get_video_mask(crtBB) .* SBef);

% do enhancement
[param.ehcBc, param.ehcBd] = init_betas(crtFrame, mask);
[editedFrame, W(:,:,1)] = modu_1st_frame(crtFrame, diffs, mask, SBef);
% flash storage
flash.frm{1}   = crtFrame;
flash.diffs{1} = diffs;
flash.mask{1}  = mask;
flash.crtBB{1} = crtBB;
% shift flash (move values "back", creating a moving window)
W = circshift(W, [0,0,1]);
flash.frm   = circshift(flash.frm,   1);
flash.diffs = circshift(flash.diffs, 1);
flash.mask  = circshift(flash.mask,  1);
flash.crtBB = circshift(flash.crtBB, 1);

% preparation for next frame
% re-generation of pyras 
pyrasAft = make_pyras(editedFrame);
% calculate gamma
rgbDiff = abs(editedFrame - crtFrame);
gamma = (1 - mean(rgbDiff(:)));
fprintf('\t%f\n', gamma);

%---SALIENCY-END---
%%

% RUN-TIME ----------------------------------------------------------------

for i = 2:length(tld.source.idx) % for every frame
    
    tld = tldProcessFrame(tld,i); % process frame i
    % tldDisplay(1,tld,i); % display results on frame i
        
    %%
    %--SALIENCY-BEGIN--
    
    crtFrame = im2double(imread(source.files(i).name));
    crtBB = [max([1;1], tld.bb(1:2,i)); ...
             min([param.resX;param.resY], tld.bb(3:4,i))];
    
    fprintf('%03d', i);

    if ~isnan(tld.valid(i))
        % prepare pyras and diffs
        lastPyrasAft = pyrasAft;
        pyrasBef = make_pyras(crtFrame, pyrasBef);
        diffs = make_diffs(pyrasBef);

        % make mask
        SBef = simple_n(enlarge(get_salimap(pyrasBef)));
        mask = simple_n(get_video_mask(crtBB) .* SBef);
        
        % do enhancement
        [editedFrame, W(:,:,1)] = ...
           modu_frame(crtFrame, diffs, mask, W(:,:,2)*gamma, lastPyrasAft);  
        % flash storage
        flash.frm{1}   = crtFrame;
        flash.diffs{1} = diffs;
        flash.mask{1}  = mask;
        flash.crtBB{1} = crtBB;
        % shift flash
        W = circshift(W, [0,0,1]);
        flash.frm   = circshift(flash.frm,   1);
        flash.diffs = circshift(flash.diffs, 1);
        flash.mask  = circshift(flash.mask,  1);
        flash.crtBB = circshift(flash.crtBB, 1);
        
        % preparation for next frame
        % re-generation of pyras 
        pyrasAft = make_pyras(editedFrame, lastPyrasAft);
        % calculate gamma
        rgbDiff = abs(editedFrame - crtFrame);
        gamma = (1 - mean(rgbDiff(:)));
        fprintf('\t%f\n', gamma);
    else
        fprintf('\ttarget lost\n');
        break;
    end
    
    if i == param.flashL
        meanW = mean(W, 3);
        iptImg = flash.frm{1};
        optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
        %         % TODO(Z)
        %         optImg=editedFrame;
        writeVideo(writerObj, optImg);
        visualHandles = init_visual(iptImg, optImg, [], [], meanW, ...
                                    flash.crtBB{1});
        % SAft = simple_n(enlarge(get_salimap(pyrasAft)));
        
        weights(:,:,weightsIdx) = meanW;
        weightsIdx = weightsIdx + 1;
        
    elseif i > param.flashL
        meanW = mean(W, 3);
        iptImg = flash.frm{1};
        optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
        %         % TODO(Z)
        %         optImg=editedFrame;
        writeVideo(writerObj, optImg);
        visualHandles = visualize(iptImg, optImg, [], [], meanW, ...
                                  flash.crtBB{1}, visualHandles);
        
        weights(:,:,weightsIdx) = meanW;
        weightsIdx = weightsIdx + 1;
        
    end
                          
    %---SALIENCY-END---
    %%
    
    if finish % finish if any key was pressed
        if tld.source.camera
            stoppreview(tld.source.vid);
            closepreview(tld.source.vid);
            close(1);
        end
        close(2);
        
        %%
        %--SALIENCY-BEGIN--
        
        close(writerObj);
        
        %---SALIENCY-END---
        %%
        
        bb = tld.bb; conf = tld.conf; % return results
        return;
    end
    
    if tld.plot.save == 1 % TODO(zori) where to set that to be also saving the plot?
        img = getframe;
        imwrite(img.cdata,[tld.output num2str(i,'%05d') '.png']);
    end
        
    
end

%%
%--SALIENCY-BEGIN--
% TODO(zori) what is 'flash'; is this the post-processing loop; how are the
% weights smoothed?
for i = 1:param.flashL
    % shift flash
    W = circshift(W, [0,0,1]);
    flash.frm   = circshift(flash.frm,   1);
    flash.diffs = circshift(flash.diffs, 1);
    flash.mask  = circshift(flash.mask,  1);
    flash.crtBB = circshift(flash.crtBB, 1);
    
    meanW = mean(W, 3);
    iptImg = flash.frm{1};
    optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
    %     % TODO(Z)
    %     optImg=editedFrame;
    writeVideo(writerObj, optImg);
    visualHandles = visualize(iptImg, optImg, [], [], meanW, ...
                              flash.crtBB{1}, visualHandles);
                          
    weights(:,:,weightsIdx) = meanW;
    weightsIdx = weightsIdx + 1;
    
end

close(writerObj);

%---SALIENCY-END---
%%

bb = tld.bb; conf = tld.conf; % return results

end
