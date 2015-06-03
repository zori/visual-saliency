% University of Surrey
%
% This file is part of TLD.
%

function [bb,conf,weights] = tldSaliency(opt)
fprintf('\nmodulation type: %s\n', int2str(opt.modulation));

global tld; % holds results and temporal variables
% allow to not visualise the modulation
% however, interrupting the processing earlier (by pressing 'q') depends on
% having the figure visible, as the key listener (handleKey) is attached to it
TO_VISUALIZE = true;

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
fig_h = 2;
figure(fig_h);
set(fig_h,'KeyPressFcn',@handle_q_key);
% by pressing 'q' key, the process will exit
global finish; % to be modified by handle_q_key()
finish = 0;

while true
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

curFrame = im2double(imread(source.files(1).name));
[param.resY, param.resX, param.nChannel] = size(curFrame);
set_param;
curBB = [max([1;1], tld.bb(1:2,1)); ...
    min([param.resX;param.resY], tld.bb(3:4,1))];

% some basic data verification
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
param.MAGICAL_DIMENSION = 6;
W = zeros(param.nEhcMaps, param.MAGICAL_DIMENSION, param.wSpan);
smoothing_param = 1 / opt.smoothing_param_recipr; % exponential smoothing param; in (0,1]
fprintf('\nsmoothing param recipr: %s; smoothing param: %f\n', num2str(opt.smoothing_param_recipr), smoothing_param);
alpha = ones(param.wSpan,1) .* smoothing_param;
for k = 2:param.wSpan
    alpha(k:end) = alpha(k:end) .* (1 - smoothing_param);
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
flash.curBB = cell(param.flashL, 1);

weights = zeros(param.nEhcMaps, param.MAGICAL_DIMENSION, param.nFrames);
weightsIdx = 1;

% writerObj = VideoWriter(strcat('result_',datestr(clock,'HHMMSS'),'.avi'));
timestamp = datestr(clock,'yyyy-mm-dd_HH-MM-SS');
if ~isempty(opt.source) && isfield(opt.source,'init_bb_name')
    % would be 'init.txt' or 'init1.txt' or 'init2.txt'...
    init_num = opt.source.init_bb_name(end-4);
    if ~isempty(str2double(init_num))
        video_name_append = [' INIT' init_num];
    end
else
    video_name_append = '';
end
video_name_append = [video_name_append ' MOD' num2str(opt.modulation)];
if opt.modulation == 3
    video_name_append = [video_name_append ' SMOOTH' num2str(opt.smoothing_param_recipr)];
end
output_video_name = [timestamp video_name_append];
output_video_path = fullfile('..', 'experiments', opt.sequence_name);
if ~exist(output_video_path,'dir')
    mkdir(output_video_path)
end
% output_flicker_saliency_txt = fullfile(output_video_path, [output_video_name ' flicker-saliency' '.txt']);

% create video writer objects for the video, the two saliency videos and the
% merged output (the three next to each other)

global n_output_videos; % to be verified in write_videos()
n_output_videos = 4;
video_writers = cell(1, n_output_videos);
% output video (modulated frames)
video_writers{1} = prepare_video_writer(fullfile(output_video_path, [output_video_name '.avi']));
% modulation saliency video
video_writers{2} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' saliency' '.avi']));
% flicker saliency video
video_writers{3} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' flicker-saliency' '.avi']));
% merged outputs of the above three
video_writers{4} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' all' '.avi']));

fprintf('\nframe \tmax ROI \tmean diff \n');
fprintf('001');

% prepare pyras and diff maps
pyrasBef = make_pyras(curFrame);
diffs = make_diffs(pyrasBef);

% make mask
% SBef = simple_n(enlarge(get_salimap(pyrasBef)));
[SBef, S_flicker] = get_salimap(pyrasBef);
SBef = simple_n(enlarge(SBef));
S_flicker = simple_n(enlarge(S_flicker));
% fprintf('#1 mean saliency (normalised, upscaled):%f\n',mean(SBef(:)));
% fprintf('#1 mean flicker saliency (as is):%f\n',mean(S_flicker(:)));
avg_flicker_saliency=zeros(length(tld.source.idx),1);
avg_flicker_saliency(1)=mean(S_flicker(:));
mask = simple_n(get_video_mask(curBB) .* SBef);

% do enhancement
[param.ehcBc, param.ehcBd] = init_betas(curFrame, mask);
% TODO(zori) figure out how modu_1st_frame is different from modu_frame and if I can merge them
[editedFrame, W(:,:,1)] = modu_1st_frame(curFrame, diffs, mask, SBef);

[flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB);

% preparation for next frame
% re-generation of pyras
pyrasAft = make_pyras(editedFrame);
% calculate gamma
rgbDiff = abs(editedFrame - curFrame);
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
    
    curFrame = im2double(imread(source.files(i).name));
    curBB = [max([1;1], tld.bb(1:2,i)); ...
        min([param.resX;param.resY], tld.bb(3:4,i))];
    
    fprintf('%03d', i);
    
    if ~isnan(tld.valid(i))
        % prepare pyras and diffs
        lastPyrasAft = pyrasAft;
        pyrasBef = make_pyras(curFrame, pyrasBef);
        diffs = make_diffs(pyrasBef);
        
        % make mask
        % SBef = simple_n(enlarge(get_salimap(pyrasBef)));
        [SBef, S_flicker] = get_salimap(pyrasBef); % TODO(zori) this is flicker BEFORE; would be the same regardless of the modulation type
        SBef = simple_n(enlarge(SBef));
        S_flicker = simple_n(enlarge(S_flicker));
        %         fprintf('#2 mean saliency (normalised, upscaled):%f\n',mean(SBef(:)));
        %         fprintf('#2 mean flicker saliency (as is):%f\n',mean(S_flicker(:)));
        avg_flicker_saliency(i)=mean(S_flicker(:));
        mask = simple_n(get_video_mask(curBB) .* SBef);
        
        % do enhancement
        [editedFrame, W(:,:,1)] = ...
            modu_frame(curFrame, diffs, mask, W(:,:,2)*gamma, lastPyrasAft);
        [flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB);
        
        % preparation for next frame
        % re-generation of pyras
        pyrasAft = make_pyras(editedFrame, lastPyrasAft);
        % calculate gamma
        rgbDiff = abs(editedFrame - curFrame);
        gamma = (1 - mean(rgbDiff(:)));
        fprintf('\t%f\n', gamma);
    else
        fprintf('\ttarget lost\n');
        break;
    end
    
    if i == param.flashL
        [input_img, output_img, meanW, weights, weightsIdx] = modulate_frame(opt.modulation, flash, W, weights, weightsIdx);

        if TO_VISUALIZE
            visualHandles = init_visual(fig_h, input_img, output_img, [], [], meanW, ...
                flash.curBB{1});
        end
        % SAft = simple_n(enlarge(get_salimap(pyrasAft)));
        
        
    elseif i > param.flashL
        [input_img, output_img, meanW, weights, weightsIdx] = modulate_frame(opt.modulation, flash, W, weights, weightsIdx);

        if TO_VISUALIZE
            visualHandles = visualize(fig_h, input_img, output_img, [], [], meanW, ...
                flash.curBB{1}, visualHandles);
        end
        write_videos(video_writers, output_img, SBef, S_flicker);
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
        on_finish(video_writers);        
        
        %---SALIENCY-END---
        %%
        
        bb = tld.bb; conf = tld.conf; % return results
        return;
    end
    
    if tld.plot.save == 1 % TODO(zori) where to set that to be also saving the plot?
        img = getframe();
        imwrite(img.cdata,[tld.output num2str(i,'%05d') '.png']);
    end
end

%%
%--SALIENCY-BEGIN--
% TODO(zori) |flash| keeps the last |param.flashL| frames from the sequence
% TODO(zori) think about the 001.png: first image never gets written; double-check that, fix if possible
for i = 1:param.flashL
    [flash, W] = shift_storage(flash, W);
    
    [input_img, output_img, meanW, weights, weightsIdx] = modulate_frame(opt.modulation, flash, W, weights, weightsIdx);

    if TO_VISUALIZE
        visualHandles = visualize(fig_h, input_img, output_img, [], [], meanW, ...
            flash.curBB{1}, visualHandles);
    end
    write_videos(video_writers, output_img, SBef, S_flicker);
end

on_finish(video_writers);

%---SALIENCY-END---
%%

bb = tld.bb; conf = tld.conf; % return results

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function handle_q_key(~,evnt)
global finish;
% disp(evt.Key)
if strcmp(evnt.Key,'q')
    finish = 1;
end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function video_writer_object = prepare_video_writer(video_name)
global param;
fprintf('\noutput video: %s\n', video_name);
video_writer_object = VideoWriter(video_name);
set(video_writer_object, 'FrameRate', param.fps);
open(video_writer_object);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB)
% flash storage
flash.frm{1}   = curFrame;
flash.diffs{1} = diffs;
flash.mask{1}  = mask;
flash.curBB{1} = curBB;
[flash, W] = shift_storage(flash, W);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [flash, W] = shift_storage(flash, W)
% shift flash (move values "back", creating a moving window)
W = circshift(W, [0,0,1]);
flash.frm   = circshift(flash.frm,   1);
flash.diffs = circshift(flash.diffs, 1);
flash.mask  = circshift(flash.mask,  1);
flash.curBB = circshift(flash.curBB, 1);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [input_img, output_img, meanW, weights, weightsIdx] = modulate_frame(modulation_type, flash, W, weights, weightsIdx)
global param;
input_img = flash.frm{1};
switch modulation_type
    case 1
        meanW = zeros(param.nEhcMaps, param.MAGICAL_DIMENSION);
        % TODO(zori): FIX bug here input and output the same!!!
        output_img = input_img; % first time over i > 5 (when i==6), |editedFrame| would be the modulated 006.png
    case 2
        meanW = mean(W, 3);
        % or, equivalently:
        % same as case 3, only alpha is a vector of length param.wSpan
        % with values 1/(param.wSpan)
        output_img = enhance(input_img, flash.diffs{1}, flash.mask{1}, meanW); % ... and this will be 002.png and it's 'enhanced' version % TODO(zori): when do we output 001.png?
    case 3
        ww = reshape(W, param.nEhcMaps*param.MAGICAL_DIMENSION, param.wSpan);
        % TODO(zori) get the alpha, make sure it is set only once, and the
        % smoothing param is available for the name of the video in the
        % beginning
        res = ww * alpha;
        meanW = reshape(res, param.nEhcMaps, param.MAGICAL_DIMENSION);
        output_img = enhance(input_img, flash.diffs{1}, flash.mask{1}, meanW);
    otherwise
        error('wrong modulation value');
end
[weights, weightsIdx] = update_weights(weights, weightsIdx, meanW);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [weights, weightsIdx] = update_weights(weights, weightsIdx, meanW)
weights(:,:,weightsIdx) = meanW;
weightsIdx = weightsIdx + 1;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function write_videos(video_writers, output_img, saliency, flicker_s)
global n_output_videos;
assert(length(video_writers) == n_output_videos);
writeVideo(video_writers{1}, output_img);
writeVideo(video_writers{2}, saliency); % saliency frame
writeVideo(video_writers{3}, flicker_s); % flicker saliency frame

cc = stitch_outputs(output_img, saliency, flicker_s); % stitch the three images together
writeVideo(video_writers{4}, cc); % the big video containing all channels
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function stitched = stitch_outputs(output_img,SBef,S_flicker)
stitched = cat(1, output_img, repmat(SBef, [1 1 3]), repmat(S_flicker, [1 1 3]));
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function on_finish(videoWriterObj,videoSaliencyWriterObj,...
%     videoFlickerSaliencyWriterObj,videosWO,output_flicker_saliency_txt,avg_flicker_saliency)
function on_finish(video_writers)
for k = 1:length(video_writers)
    close(video_writers{k});
end

% dlmwrite(output_flicker_saliency_txt,mean(avg_flicker_saliency),'delimiter',' ');
% dlmwrite(output_flicker_saliency_txt,avg_flicker_saliency','-append',...
%     'roffset',1,'delimiter',' ');
end
