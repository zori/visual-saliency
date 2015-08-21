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

%--SALIENCY-BEGIN--
%
global param;
param = {};
param.modulation_type = opt.modulation;
modulation_type_str = int2str(param.modulation_type);
fprintf('\nmodulation type: %s\n', modulation_type_str);
%---SALIENCY-END---

% INITIALIZATION ----------------------------------------------------------

% select data source, camera/directory
opt.source = tldInitSource(opt.source);

% open figure for display of results
FIG_H = 2;
figure(FIG_H);
set(FIG_H,'KeyPressFcn',@handle_q_key);
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

%--SALIENCY-BEGIN--
curFrame = im2double(imread(source.files(1).name));
[param.resY, param.resX, param.nChannel] = size(curFrame);
% NOTE(zori): bad practice ahead: param is global, being changed in the following
set_param();
assert(param.nFrames == length(tld.source.idx));
curBB = get_current_BB(1);

% some basic data verification
if param.nChannel < 3
    disp('Color image expected. Process terminated.')
    return
elseif min(param.resY, param.resX) < 2^param.pyraScales
    disp('Minimise resolution should be 2^(scale of pyramid).');
    return
elseif param.nFrames < param.wSpan
    disp('Video too short.')
    return
end

W = zeros(param.nEhcMaps, param.CHANNEL_WEIGHTS_DIM, param.wSpan);

flash = {};
flash.frm   = cell(param.flashL, 1);
flash.diffs = cell(param.flashL, 1);
flash.mask  = cell(param.flashL, 1);
flash.curBB = cell(param.flashL, 1);

weights = zeros(param.nEhcMaps, param.CHANNEL_WEIGHTS_DIM, param.nFrames);
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

% add (W)LLS option and AREA option to output name
minim_opt.within_frame = MinimisationOption.T_ORIG; % T_DLLS;
minim_opt.temporal = MinimisationOption.TEMP_W; % TEMP_O;
minim_opt.area = MinimisationOption.A_IMG;

video_name_append = [video_name_append ' ' char(minim_opt.within_frame)];
video_name_append = [video_name_append ' ' char(minim_opt.temporal)];
video_name_append = [video_name_append ' ' char(minim_opt.area)];

output_video_name = [timestamp video_name_append];
output_video_path = fullfile('..', 'experiments', opt.sequence_name);
if ~exist(output_video_path, 'dir')
    mkdir(output_video_path)
end
saliency_flicker_writers{1}.txt = fullfile(output_video_path, [output_video_name ' saliency-flicker' '.txt']);
saliency_flicker_writers{2}.txt = fullfile(output_video_path, ['saliency-flicker' '.txt']);
saliency_flicker_writers{3}.txt = fullfile(output_video_path, [output_video_name ' saliency-plot']);
saliency_flicker_writers{4}.txt = fullfile(output_video_path, [output_video_name ' saliency-flicker-plot']);

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
% diffs - [param.nEhcMaps x 3] cell of difference maps
%         first column: I
%         second column: R and G
%         third column: B and Y
diffs = make_diffs(pyrasBef);

% make mask
saliency_flicker_writers{2}.avg = zeros(param.nFrames, 1);
[writable_imgs{6}, writable_imgs{7}, saliency_flicker_writers{2}.avg(1)] = ...
    pyras2saliency(pyrasBef);
mask = get_mask(curBB, writable_imgs{6});
% do enhancement
[param.ehcBc, param.ehcBd] = init_betas(curFrame, mask);
[editedFrame, W(:,:,1)] = modu_1st_frame(curFrame, diffs, mask, writable_imgs{6});

[flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB);

% preparation for next frame
% re-generation of pyras
saliency_flicker_writers{1}.avg = zeros(param.nFrames, 1);
saliency_flicker_writers{1}.avg_abs = zeros(param.nFrames, 1);
pyrasAft = make_pyras(editedFrame);
prev_edited_frame = editedFrame;
pyras_modulated = pyrasAft;
[writable_imgs{2}, writable_imgs{3}, saliency_flicker_writers{1}.avg(1), saliency_flicker_writers{1}.avg_abs(1)] = ...
    pyras2saliency(pyras_modulated, writable_imgs{7});
% calculate gamma
rgbDiff = abs(editedFrame - curFrame);
gamma = (1 - mean(rgbDiff(:)));
fprintf('\t%f\n', gamma);

%---SALIENCY-END---

% RUN-TIME ----------------------------------------------------------------

for k = 2:param.nFrames % for every frame
    % i = 2; % TODO(zori): sanity check; always use 002.png
    i = k;
    tld = tldProcessFrame(tld,i); % process frame i
    % tldDisplay(1,tld,i); % display results on frame i
    
    %--SALIENCY-BEGIN--
    curFrame = im2double(imread(source.files(i).name));
    curBB = get_current_BB(i);
    
    fprintf('%03d', k);
    
    if ~isnan(tld.valid(i))
        % prepare pyras and diffs
        lastPyrasAft = pyrasAft;
        pyrasBef = make_pyras(curFrame, pyrasBef);
        diffs = make_diffs(pyrasBef);
        
        % make mask
        [writable_imgs{6}, writable_imgs{7}, saliency_flicker_writers{2}.avg(k)] = ...
            pyras2saliency(pyrasBef);
        mask = get_mask(curBB, writable_imgs{6});
        
        % do enhancement
        [editedFrame, W(:,:,1)] = modu_frame(curFrame, k, diffs, mask,...
            W(:,:,2)*gamma, lastPyrasAft, minim_opt, prev_edited_frame,...
            writable_imgs{6});
        [flash, W] = store_and_shift(flash, W, curFrame, diffs, mask, curBB);
        
        % preparation for next frame
        prev_edited_frame = editedFrame;
        % re-generation of pyras
        pyrasAft = make_pyras(editedFrame, lastPyrasAft);
        % calculate gamma
        rgbDiff = abs(editedFrame - curFrame);
        gamma = (1 - mean(rgbDiff(:)));
        fprintf('\t%f\n', gamma);
    else
        fprintf('\ttarget lost\n');
        exit('Error: target lost');
    end
    
    if k == param.flashL
        [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = smooth_frame_modulation(flash, W, weights, weightsIdx);
        
        if TO_VISUALIZE
            % % save the image of first frame with the bounding box in the
            % % original video's folder (only once)
            % ff = figure('visible', 'off'); imshow(input_img, 'Border', 'tight');
            % BB = flash.curBB{1}; rectangle('Position', [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)], 'LineWidth', 4, 'EdgeColor', 'y');
            % print(ff, '-r80', '-djpeg', fullfile(source.input, [opt.source.init_bb_name(1:end-4) '.jpg']));
            
            visualHandles = init_visual(FIG_H, input_img, writable_imgs{1}, [], [], meanW, ...
                flash.curBB{1});
        end
        % TODO(zori) pyrasAft code repetition? Is this the modulated frame's
        % pyramids?
        % SAft = simple_norm(enlarge(get_salimap(pyrasAft)));

    elseif k > param.flashL
        [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = smooth_frame_modulation(flash, W, weights, weightsIdx);
        
        if TO_VISUALIZE
            visualHandles = visualize(FIG_H, input_img, writable_imgs{1}, [], [], meanW, ...
                flash.curBB{1}, visualHandles);
        end
        % TOOD(zori): start from here; what should pyras_modulated be?
        % pyras_modulated = pyrasAft;
        pyras_modulated = make_pyras(writable_imgs{1}, pyrasBef); % pyras_modulated);
        kk = k - param.flashL + 1;
        [writable_imgs{2}, writable_imgs{3}, saliency_flicker_writers{1}.avg(kk), saliency_flicker_writers{1}.avg_abs(kk)] = ...
            pyras2saliency(pyras_modulated, writable_imgs{7});
        if saliency_flicker_writers{1}.avg_abs(kk) <= saliency_flicker_writers{2}.avg(kk)
            % TODO(zori) this happens too often; how is this possible in the
            % first place? Maybe that indicates a bug in the flicker saliency
            % computation
            % warning(['(at k) modulated saliency flicker lower than original one (' num2str(saliency_flicker_writers{1}.avg_abs(kk)) ' and ' num2str(saliency_flicker_writers{2}.avg(k)) ')']);
        end
        if param.write_orig, writable_imgs{5} = input_img; end
        write_videos(video_writers, writable_imgs);
    end
    
    %---SALIENCY-END---
    
    if finish % finish if any key was pressed
        if tld.source.camera
            stoppreview(tld.source.vid);
            closepreview(tld.source.vid);
            close(1);
        end
        close(2);
        
        %--SALIENCY-BEGIN--
        on_finish(video_writers, saliency_flicker_writers, k);
        
        %---SALIENCY-END---
        
        bb = tld.bb; conf = tld.conf; % return results
        return;
    end
    
    if tld.plot.save == 1 % TODO(zori) where to set that to be also saving the plot?
        img = getframe();
        imwrite(img.cdata,[tld.output num2str(k,'%05d') '.png']);
    end
end

%--SALIENCY-BEGIN--
% TODO(zori) think about the 001.png: first image never gets written; double-check that, fix if possible
for i = 1:param.flashL % |flash| keeps the last |param.flashL| frames from the sequence
    ind = i + param.nFrames - param.flashL;
    [flash, W] = shift_storage(flash, W);
    
    [input_img, writable_imgs{1}, meanW, weights, weightsIdx] = smooth_frame_modulation(flash, W, weights, weightsIdx);

    pyrasBef = make_pyras(input_img, pyrasBef);
    [writable_imgs{6}, writable_imgs{7}, saliency_flicker_writers{2}.avg(ind)] = ...
        pyras2saliency(pyrasBef);
    
    if TO_VISUALIZE
        visualHandles = visualize(FIG_H, input_img, writable_imgs{1}, [], [], meanW, ...
            flash.curBB{1}, visualHandles);
    end
    % pyras_modulated = pyrasBef; % TODO(zori) I think yes
    pyras_modulated = make_pyras(writable_imgs{1}, pyrasBef);
    [writable_imgs{2}, writable_imgs{3}, saliency_flicker_writers{1}.avg(ind), saliency_flicker_writers{1}.avg_abs(ind)] = ...
        pyras2saliency(pyras_modulated, writable_imgs{7});
    if saliency_flicker_writers{1}.avg_abs(ind) <= saliency_flicker_writers{2}.avg(ind)
        % warning('(at ind) modulated saliency flicker lower than original one');
    end
    if param.write_orig, writable_imgs{5} = input_img; end
    write_videos(video_writers, writable_imgs);
end

% video modulation was not interrupted before the last frame
assert(k == param.nFrames);
on_finish(video_writers, saliency_flicker_writers, param.nFrames);

%---SALIENCY-END---

bb = tld.bb; conf = tld.conf; % return results

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function current_BB = get_current_BB(ind)
global tld;
global param;
% a column vector with the bb coords for frame #ind: tld.bb(:,ind)
current_BB = [max([1;1], tld.bb(1:2,ind)); ...
    min([param.resX;param.resY], tld.bb(3:4,ind))];

% % Single-pixel experiment: get the upper-right corner instead
% x2 = tld.bb(3, ind);
% y1 = tld.bb(2, ind);
% current_BB = [x2 y1 x2 y1]';
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function mask = get_mask(current_BB, saliency)
mask = simple_norm(get_video_mask(current_BB) .* saliency);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function handle_q_key(~,evnt)
global finish;
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
function [input_img, output_img, meanW, weights, weightsIdx] = smooth_frame_modulation(flash, W, weights, weightsIdx)
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
        W_reshaped = reshape(W, param.nEhcMaps * param.CHANNEL_WEIGHTS_DIM, param.wSpan);
        assert(isfield(param, 'alpha'));
        res = W_reshaped * param.alpha;
        meanW = reshape(res, param.nEhcMaps, param.CHANNEL_WEIGHTS_DIM);
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
writable_imgs{3} = saliency_flicker_visualise(writable_imgs{3});
% writable_imgs{4} = stitch_outputs(writable_imgs{1}, writable_imgs{2}, saliency_flicker_visualise(writable_imgs{3}));
% writable_imgs{8} = stitch_outputs(writable_imgs{5}, writable_imgs{6}, writable_imgs{7});
for k = 1:param.n_batches
    l = (k-1) * param.n_videos_in_batch + 2;
    stitched = stitch_outputs(writable_imgs{l-1}, writable_imgs{l}, writable_imgs{l+1}); % stitch the three images together
    writable_imgs{k * param.n_videos_in_batch} = stitched; % the big video containing all channels
end
write_videos_do(video_writers, writable_imgs);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function saliency_flicker_colour = saliency_flicker_visualise(saliency_flicker_diff)
% writable_imgs{3} is the modulated flicker saliency
% writable_imgs{7} is the original video flicker saliency (before modulation)
% for better viewing, subtract from the modulated the original, and depict:
%   red pixel: positive value (i.e, modulated value is higher),
%   white pixel: 0 (no difference),
%   blue pixel: negative value (i.e, modulated value is lower),
%
% R = v .* (v > 0)
% G = 0
% B = (-v) .* (v < 0)
[r, c] = size(saliency_flicker_diff);
% dynamic range adaptation: make the highest value most red / blue and
% everything else linearly scaled
saliency_flicker_diff = simple_norm(saliency_flicker_diff) .* 2 - 1;
equal_values = ones(r, c) .* (abs(saliency_flicker_diff) < eps);
% % for testing:
% equal_values(24300:24940)=1; im(equal_values);
equal_values_3D = repmat(equal_values, [1, 1, 3]);
diff_values = cat(3, ...
    saliency_flicker_diff .* (saliency_flicker_diff > 0),... % R
    zeros(r, c),... % G
    -saliency_flicker_diff .* (saliency_flicker_diff < 0)); % B
saliency_flicker_colour = ~equal_values_3D .* diff_values + equal_values_3D;
% figure; im(saliency_flicker_colour);
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
% saliency is greyscale
% saliency_flicker is greyscale for the original (unmodulated) video, and RGB
% for the modulated, to ease visualisation
if ismatrix(saliency_flicker) % greyscale image
    saliency_flicker_rgb = repmat(saliency_flicker, [1 1 3]);
else
    if ndims(saliency_flicker) == 3 % RGB
        saliency_flicker_rgb = saliency_flicker;
    else
        error('invalid saliency_flicker image');
    end
end
stitched = cat(1, output_img, repmat(saliency, [1 1 3]), saliency_flicker_rgb);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function on_finish(video_writers, saliency_flicker_writers, cur_frame_number)
global param;
% % number of boost_HSI() calls made (out of cur_frame_number - 1, as the first
% frame is not modulated). When execution is not terminated before the last
% frame of the video, cur_frame_number == param.nFrames
max_sal_outside_ROI = cur_frame_number - 1 - sum(param.ROI_sal_max);
fprintf('number of frames w/ max saliency outside ROI: %d\n', max_sal_outside_ROI);

% TODO(zori) convert all writes to file from dlmwrite to fprintf as it's only
% meant to be human-readable

% % saliency plot
% ROI avg saliency
SAL_PLOT_FIG_H = 3;
figure(SAL_PLOT_FIG_H);
avg = param.ROI_sal ./ param.nROI_pixels;
plot(avg, 'g');
hold on
boosting_req = double(~param.ROI_sal_max);
boosting_req(boosting_req == 0) = NaN;
boosted = avg .* (boosting_req);
% boosted(boosted == 0) = NaN;
plot(boosted, 'b*');
max_non_ROI_sal_val = param.max_non_ROI_sal_val;
max_non_ROI_sal_val(max_non_ROI_sal_val == 0) = NaN;
plot(max_non_ROI_sal_val, 'r');
hold off

xlabel('frame number (time)');
ylabel('saliency');
title('saliency');
legend('avg ROI', 'boosting', 'max outside ROI', 'Location', 'southeast');

saveas(gcf, saliency_flicker_writers{3}.txt, 'jpg');

% % saliency_flicker plot
SAL_FLICKER_PLOT_FIG_H = 4;
figure(SAL_FLICKER_PLOT_FIG_H);
orig = saliency_flicker_writers{2}.avg;
orig(orig == 0) = NaN;
plot(orig, 'c')
hold on
% plot(boosting_req .* orig', 'rx');
% bar(boosting_req, 0.03);
modulated_abs = saliency_flicker_writers{1}.avg_abs;
modulated_abs(modulated_abs == 0) = NaN;
modu_b = boosting_req .* modulated_abs';
orig_b = boosting_req .* orig';
plot(modulated_abs, 'm')
v = boosting_req .* (1:param.nFrames);
for k = 1:length(v) 
    plot([v(k), v(k)], [modu_b(k), orig_b(k)], 'r');
end
hold off

xlabel('frame number (time)');
ylabel('saliency flicker');
title('saliency flicker');
legend('original', 'modulated', 'boosting', 'Location', 'southeast');

saveas(gcf, saliency_flicker_writers{4}.txt, 'jpg');

% % stats
dlmwrite(saliency_flicker_writers{1}.txt, max_sal_outside_ROI);
for k = 1:param.n_batches
    frame_avg = saliency_flicker_writers{k}.avg; % vector of length param.nFrames with the avg saliency_flicker values per frame (absolute or relative to the original, for the modulated)
    dlmwrite(saliency_flicker_writers{k}.txt,...
        [mean(frame_avg)...
        std(frame_avg)...
        ], '-append', 'roffset', 1);
end

dlmwrite(saliency_flicker_writers{1}.txt,...
    [param.frames_out_of_range_modulated_vals...
    param.num_out_of_range_modulated_vals...
    ], '-append', 'roffset', 1);

assert(length(video_writers) == param.n_output_videos);
for k = 1:param.n_output_videos
    close(video_writers{k});
end
for k = 1:param.n_batches
    dlmwrite(saliency_flicker_writers{k}.txt, saliency_flicker_writers{k}.avg', ...
        '-append', 'roffset', 1, 'delimiter', ' ');
end
end
