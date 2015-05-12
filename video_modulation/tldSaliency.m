% University of Surrey
%
% This file is part of TLD.
%

function [bb,conf,weights] = tldSaliency(opt)
% three modulation options:
% 1) no modulation (would result in flicker)
% 2) video modulation with simple moving average (as Tao SHI originally implemented)
% 3) exponential smoothing with smoothing weight alpha
MODULATION=1;
fprintf('\nMODULATION type: %s\n', int2str(MODULATION));

global tld; % holds results and temporal variables
% allow to not visualise the modulation
% however, interrupting the processing earlier (by pressing 'q') depends on
% having the figure visible, as the key listener (handleKey) is attached to it
TO_VISUALIZE=true;

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
fig_h=2;
figure(fig_h);
set(fig_h,'KeyPressFcn',@handle_q_key);
% by pressing 'q' key, the process will exit
global finish;
finish = 0;

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
% TODO(zori) where is the "magical 6" taken from
MAGICAL_DIMENSION=6;
W = zeros(param.nEhcMaps, MAGICAL_DIMENSION, param.wSpan);
smoothing_param_recipr=3;
smoothing_param = 1/smoothing_param_recipr; % exponential smoothing param; in (0,1]
fprintf('\nsmoothing param recipr: %s; smoothing param: %f\n', num2str(smoothing_param_recipr), smoothing_param);
alpha=ones(param.wSpan,1).*smoothing_param;
for k=2:param.wSpan
    alpha(k:end)=alpha(k:end).*(1-smoothing_param);
end
% for smoothing_param = 1/3:
% alpha: 0.3333    0.2222    0.1481    0.0988    0.0658    0.0439    0.0293    0.0195    0.0130
%
% for smoothing_param = 1/6:
% alpha: 0.1667    0.1389    0.1157    0.0965    0.0804    0.0670    0.0558    0.0465    0.0388
%
% for smoothing_param = 1/7:
% alpha: 0.1429    0.1224    0.1050    0.0900    0.0771    0.0661    0.0567    0.0486    0.0416
%
% for smoothing_param = 1/9:
% 0.1111    0.0988    0.0878    0.0780    0.0694    0.0617    0.0548    0.0487    0.0433

flash = {};
flash.frm   = cell(param.flashL, 1);
flash.diffs = cell(param.flashL, 1);
flash.mask  = cell(param.flashL, 1);
flash.crtBB = cell(param.flashL, 1);

weights = zeros(param.nEhcMaps, 6, param.nFrames);
weightsIdx = 1;

% writerObj = VideoWriter(strcat('result_',datestr(clock,'HHMMSS'),'.avi'));
timestamp = datestr(clock,'yyyy-mm-dd_HH-MM-SS');
video_name_append=[' MOD' num2str(MODULATION)];
if MODULATION==3
    video_name_append=[video_name_append ' SMOOTH' num2str(smoothing_param_recipr)];
end
output_video_name = [timestamp video_name_append '.avi'];
output_video_path = fullfile('..', 'experiments', opt.sequence_name);
if ~exist(output_video_path,'dir')
    mkdir(output_video_path)
end
output_video = fullfile(output_video_path, output_video_name);
fprintf('\noutput video is: %s\n', output_video);
writerObj = VideoWriter(output_video);
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
        iptImg = flash.frm{1};
        switch MODULATION
            case 1
                optImg=editedFrame;
                meanW = zeros(param.nEhcMaps, MAGICAL_DIMENSION);
            case 2
                meanW = mean(W, 3);
                % or, equivalently:
                % same as case 3, only alpha is a vector of length param.wSpan
                % with values 1.param.wSpan
                optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
            case 3
                ww=reshape(W, param.nEhcMaps*MAGICAL_DIMENSION, param.wSpan);
                res=ww*alpha;
                meanW=reshape(res, param.nEhcMaps, MAGICAL_DIMENSION);
                optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
            otherwise
                error('wrong modulation value');
        end
        if TO_VISUALIZE
            visualHandles = init_visual(fig_h, iptImg, optImg, [], [], meanW, ...
                                    flash.crtBB{1});
        end
        % SAft = simple_n(enlarge(get_salimap(pyrasAft)));
        
        weights(:,:,weightsIdx) = meanW;
        weightsIdx = weightsIdx + 1;
        
    elseif i > param.flashL
        iptImg = flash.frm{1};
        switch MODULATION
            case 1
                meanW = zeros(param.nEhcMaps, MAGICAL_DIMENSION);
                optImg=editedFrame;
            case 2
                meanW = mean(W, 3);
                % or, equivalently:
                % same as case 3, only alpha is a vector of length param.wSpan
                % with values 1.param.wSpan
                optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
            case 3
                ww=reshape(W, param.nEhcMaps*MAGICAL_DIMENSION, param.wSpan);
                res=ww*alpha;
                meanW=reshape(res, param.nEhcMaps, MAGICAL_DIMENSION);
                optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
            otherwise
                error('wrong modulation value');
        end
        writeVideo(writerObj, optImg);
        if TO_VISUALIZE
            visualHandles = visualize(fig_h, iptImg, optImg, [], [], meanW, ...
                                  flash.crtBB{1}, visualHandles);
        end
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
    
    iptImg = flash.frm{1};
    switch MODULATION
        case 1
            optImg=editedFrame;
            meanW = zeros(param.nEhcMaps, MAGICAL_DIMENSION);
        case 2
            meanW = mean(W, 3);
            % or, equivalently:
            % same as case 3, only alpha is a vector of length param.wSpan
            % with values 1.param.wSpan
            optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
        case 3
            ww=reshape(W, param.nEhcMaps*MAGICAL_DIMENSION, param.wSpan);
            res=ww*alpha;
            meanW=reshape(res, param.nEhcMaps, MAGICAL_DIMENSION);
            optImg = enhance(iptImg, flash.diffs{1}, flash.mask{1}, meanW);
        otherwise
            error('wrong modulation value');
    end
    writeVideo(writerObj, optImg);
    if TO_VISUALIZE
        visualHandles = visualize(fig_h, iptImg, optImg, [], [], meanW, ...
                              flash.crtBB{1}, visualHandles);
    end
    weights(:,:,weightsIdx) = meanW;
    weightsIdx = weightsIdx + 1;
    
end

close(writerObj);

%---SALIENCY-END---
%%

bb = tld.bb; conf = tld.conf; % return results

end

function handle_q_key(~,evnt)
global finish;
% disp(evt.Key)
if strcmp(evnt.Key,'q')
    finish = 1;
end
end

