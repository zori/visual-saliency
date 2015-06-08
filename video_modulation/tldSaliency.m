% University of Surrey
%
% This file is part of TLD.
%

function [bb,conf,weights] = tldSaliency(opt)
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
param.modulation_type = opt.modulation;
modulation_type_str = int2str(param.modulation_type);
fprintf('\nmodulation type: %s\n', modulation_type_str);
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
video_name_append = [video_name_append ' MOD' modulation_type_str];
if param.modulation_type == MOD_TYPE_EXP_SMOOTHING
    param.smoothing_param = 1 / opt.smoothing_param_recipr; % exponential smoothing param; in (0,1]
    fprintf('\nsmoothing param recipr: %s; smoothing param: %f\n', num2str(opt.smoothing_param_recipr), param.smoothing_param);
    param.alpha = ones(param.wSpan,1) .* param.smoothing_param;
    for k = 2:param.wSpan
        param.alpha(k:end) = param.alpha(k:end) .* (1 - param.smoothing_param);
    end
    % for alpha.smoothing_param = 1/3:
    % alpha: 0.3333    0.2222    0.1481    0.0988    0.0658    0.0439    0.0293    0.0195    0.0130
    %
    % for alpha.smoothing_param = 1/6:
    % alpha: 0.1667    0.1389    0.1157    0.0965    0.0804    0.0670    0.0558    0.0465    0.0388
    %
    % for alpha.smoothing_param = 1/7:
    % alpha: 0.1429    0.1224    0.1050    0.0900    0.0771    0.0661    0.0567    0.0486    0.0416
    %
    % for alpha.smoothing_param = 1/9:
    % 0.1111    0.0988    0.0878    0.0780    0.0694    0.0617    0.0548    0.0487    0.0433
    
    video_name_append = [video_name_append ' SMOOTH' num2str(opt.smoothing_param_recipr)];
end
output_video_name = [timestamp video_name_append];
output_video_path = fullfile('..', 'experiments', opt.sequence_name);
if ~exist(output_video_path, 'dir')
    mkdir(output_video_path)
end
saliency_flicker_writers{1}.txt = fullfile(output_video_path, [output_video_name ' saliency-flicker' '.txt']);
saliency_flicker_writers{2}.txt = fullfile(output_video_path, ['saliency-flicker' '.txt']);

% create video writer objects for the video, the two saliency videos and the
% merged output (the three next to each other)

% check whether the original video's statistic (saliency, flicker saliency,
% flicker saliency file summary) is already there

param.n_videos_in_batch = 4;
param.write_orig = ~exist(saliency_flicker_writers{2}.txt, 'file');
if ~param.write_orig
    param.n_batches = 1;
else
    param.n_batches = 2;
end
param.n_output_videos = param.n_videos_in_batch * param.n_batches; %  to be verified in write_videos()

video_writers = cell(1, param.n_output_videos);
writable_imgs = cell(1, param.n_output_videos);
% output video (modulated frames)
video_writers{1} = prepare_video_writer(fullfile(output_video_path, [output_video_name '.avi']));
% modulation saliency video
video_writers{2} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' saliency' '.avi']));
% flicker saliency video
video_writers{3} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' saliency-flicker' '.avi']));
% merged outputs of the above three
video_writers{4} = prepare_video_writer(fullfile(output_video_path, [output_video_name ' all' '.avi']));

if param.write_orig
    video_writers{5} = prepare_video_writer(fullfile(output_video_path, 'orig.avi'));
    video_writers{6} = prepare_video_writer(fullfile(output_video_path, ['saliency' '.avi']));
    video_writers{7} = prepare_video_writer(fullfile(output_video_path, ['saliency-flicker' '.avi']));
    video_writers{8} = prepare_video_writer(fullfile(output_video_path, ['all' '.avi']));
end

fprintf('\nframe \tmax ROI \tmean diff \n');
fprintf('001');

% prepare pyras and diff maps
pyrasBef = make_pyras(curFrame);
diffs = make_diffs(pyrasBef);

% make mask
% SBef = simple_n(enlarge(get_salimap(pyrasBef)));
[SBef, S_flicker] = get_salimap(pyrasBef);
[writable_imgs{6}, writable_imgs{7}] = process_saliency(SBef, S_flicker);
% fprintf('#1 mean saliency (normalised, upscaled):%f\n',mean(SBef(:)));
% fprintf('#1 mean flicker saliency (as is):%f\n',mean(S_flicker(:)));

saliency_flicker_writers{2}.avg = zeros(length(tld.source.idx), 1);
saliency_flicker_writers{2}.avg(1) = mean(writable_imgs{7}(:));
mask = simple_n(get_video_mask(curBB) .* writable_imgs{6});

% do enhancement
[param.ehcBc, param.ehcBd] = init_betas(curFrame, mask);
% TODO(zori) figure out how modu_1st_frame is different from modu_frame and if I can merge them
[editedFrame, W(:,:,1)] = modu_1st_frame(curFrame, diffs, mask, writable_imgs{6});

[flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB);

% preparation for next frame
% re-generation of pyras
pyrasAft = make_pyras(editedFrame);
pyras_modulated = pyrasAft;
[s_modulated, s_flicker_modulated] = get_salimap(pyras_modulated);
[writable_imgs{2}, writable_imgs{3}] = process_saliency(s_modulated, s_flicker_modulated);
saliency_flicker_writers{1}.avg = zeros(length(tld.source.idx), 1);
saliency_flicker_writers{1}.avg(1) = mean(writable_imgs{3}(:));
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
        [writable_imgs{6}, writable_imgs{7}] = process_saliency(SBef, S_flicker);
        %         fprintf('#2 mean saliency (normalised, upscaled):%f\n',mean(SBef(:)));
        %         fprintf('#2 mean flicker saliency (as is):%f\n',mean(S_flicker(:)));
        saliency_flicker_writers{2}.avg(i) = mean(writable_imgs{7}(:));
        mask = simple_n(get_video_mask(curBB) .* writable_imgs{6});

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
        [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = modulate_frame(flash, W, weights, weightsIdx);

        if TO_VISUALIZE
            % % save the image of first frame with the bounding box in the
            % % original video's folder (only once)
            % ff = figure('visible', 'off'); imshow(input_img, 'Border', 'tight');
            % BB = flash.curBB{1}; rectangle('Position', [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)], 'LineWidth', 4, 'EdgeColor', 'y');
            % print(ff, '-r80', '-djpeg', fullfile(source.input, [opt.source.init_bb_name(1:end-4) '.jpg']));

            visualHandles = init_visual(fig_h, input_img, writable_imgs{1}, [], [], meanW, ...
                flash.curBB{1});
        end
        % SAft = simple_n(enlarge(get_salimap(pyrasAft)));
        
        
    elseif i > param.flashL
        [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = modulate_frame(flash, W, weights, weightsIdx);

        if TO_VISUALIZE
            visualHandles = visualize(fig_h, input_img, writable_imgs{1}, [], [], meanW, ...
                flash.curBB{1}, visualHandles);
        end
        pyras_modulated = make_pyras(writable_imgs{1}, pyras_modulated);
        [s_modulated, s_flicker_modulated] = get_salimap(pyras_modulated);
        [writable_imgs{2}, writable_imgs{3}] = process_saliency(s_modulated, s_flicker_modulated);
        saliency_flicker_writers{1}.avg(i) = mean(writable_imgs{3}(:));
        if param.write_orig, writable_imgs{5} = input_img; end
        write_videos(video_writers, writable_imgs);
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
        on_finish(video_writers, saliency_flicker_writers);
        
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
    
    [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = modulate_frame(flash, W, weights, weightsIdx);

    if TO_VISUALIZE
        visualHandles = visualize(fig_h, input_img, writable_imgs{1}, [], [], meanW, ...
            flash.curBB{1}, visualHandles);
    end
    pyras_modulated = make_pyras(writable_imgs{1}, pyras_modulated);
    [s_modulated, s_flicker_modulated] = get_salimap(pyras_modulated);
    [writable_imgs{2}, writable_imgs{3}] = process_saliency(s_modulated, s_flicker_modulated);
    saliency_flicker_writers{1}.avg(i) = mean(writable_imgs{3}(:));
    if param.write_orig, writable_imgs{5} = input_img; end
    write_videos(video_writers, writable_imgs);
end

on_finish(video_writers, saliency_flicker_writers);

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
function varargout = process_saliency(varargin)
varargout = cell(size(varargin));
for k=1:nargin
    varargout{k} = simple_n(enlarge(varargin{k}));
end
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
function [input_img, output_img, meanW, weights, weightsIdx] = modulate_frame(flash, W, weights, weightsIdx)
global param;
input_img = flash.frm{1};
switch param.modulation_type
    case 1
        meanW = W(:,:,2); % just get the last weights, no averaging; will result in flicker
    case 2
        meanW = mean(W, 3);
        % or, equivalently:
        % same as case 3, only alpha is a vector of length param.wSpan
        % with values 1/(param.wSpan)
    case MOD_TYPE_EXP_SMOOTHING
        W_reshaped = reshape(W, param.nEhcMaps * param.MAGICAL_DIMENSION, param.wSpan);
        assert(isfield(param, 'alpha'));
        res = W_reshaped * param.alpha;
        meanW = reshape(res, param.nEhcMaps, param.MAGICAL_DIMENSION);
    otherwise
        error('wrong modulation value');
end
output_img = enhance(input_img, flash.diffs{1}, flash.mask{1}, meanW);
[weights, weightsIdx] = update_weights(weights, weightsIdx, meanW);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [weights, weightsIdx] = update_weights(weights, weightsIdx, meanW)
weights(:,:,weightsIdx) = meanW;
weightsIdx = weightsIdx + 1;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function write_videos(video_writers, writable_imgs)
global param;
assert(length(video_writers) == param.n_output_videos);
for k = 1:param.n_batches
    l = (k-1) * param.n_videos_in_batch + 2;
    stitched = stitch_outputs(writable_imgs{l-1}, writable_imgs{l}, writable_imgs{l+1}); % stitch the three images together
    writable_imgs{k * param.n_videos_in_batch} = stitched; % the big video containing all channels
end
write_videos_do(video_writers, writable_imgs);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function write_videos_do(video_writers, writable_imgs)
global param;
% assert(length(writable_imgs) == param.n_output_videos);
for k = 1:param.n_output_videos
    writeVideo(video_writers{k}, writable_imgs{k});
end
% writeVideo(video_writers{1}, output_img);
% writeVideo(video_writers{2}, saliency); % saliency frame
% writeVideo(video_writers{3}, flicker_s); % flicker saliency frame
% cc = stitch_outputs(output_img, saliency, flicker_s); % stitch the three images together
% writeVideo(video_writers{4}, cc); % the big video containing all channels
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function stitched = stitch_outputs(output_img, saliency, saliency_flicker)
% stitch_outputs(output_img,SBef,S_flicker)
stitched = cat(1, output_img, repmat(saliency, [1 1 3]), repmat(saliency_flicker, [1 1 3]));
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function on_finish(video_writers, saliency_flicker_writers)
global param;
assert(length(video_writers) == param.n_output_videos);
for k = 1:param.n_output_videos
    close(video_writers{k});
end
for k = 1:param.n_batches
    dlmwrite(saliency_flicker_writers{k}.txt, mean(saliency_flicker_writers{k}.avg), 'delimiter', ' ');
    dlmwrite(saliency_flicker_writers{k}.txt, saliency_flicker_writers{k}.avg', ...
        '-append', 'roffset', 1, 'delimiter', ' ');
end
end
