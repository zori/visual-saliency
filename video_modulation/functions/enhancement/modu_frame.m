function [ frame_out, W ] = modu_frame(frame_orig, frame_ind, diffs, mask,...
    W_orig, lastPyrasAft, minim_opt, frame_prev_edited, cur_modu_saliency)
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao

global param;

% pre-enhance
frame_enhanced = enhance(frame_orig, diffs, mask, W_orig);
pyrasAft = make_pyras(frame_enhanced, lastPyrasAft);
SAft = process_saliency(get_salimap(pyrasAft));

% % mark the location of max saliency after the modulation
% figure; im(SAft); [~, maxind] = max(SAft(:)); [ri,ci] = ind2sub([r, c], maxind); hold on; plot(ci, ri, 'x');

% prepare ehcA
mask_locations = mask ~= 0;
SROI  = SAft(mask_locations);
% ROI_n = length(SROI);
maxROI = max(SROI);
% param.modu_frame_calls = param.modu_frame_calls + 1;
param.max_non_ROI_sal_val(frame_ind) = max(SAft(~mask_locations));
param.max_non_ROI_sal_ind(frame_ind) = find(SAft == 1, 1);
% disp(length(find(SAft == 1))); % how many max salient pixels are there
% compute avg by dividing the following two:
param.ROI_sal(frame_ind) = sum(SROI);
param.nROI_pixels(frame_ind) = length(SROI);
param.ROI_sal_max(frame_ind) = maxROI == 1;
% get a few frames with the locations of the max-salient out-of-ROI pixel
if maxROI ~= 1 % mobile INIT1 MOD1 frame 016 - 038 no boosting; 039 boosting
    % disp('boosting req');
    % db_stop;
    %         [y, x] = ind2sub(size(SAft), param.max_non_ROI_sal_ind(frame_ind));
    %         figure(7); im(frame_orig); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_f'], 'jpg');
    %         figure(8); im(SAft); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_SAft'], 'jpg');
end
fprintf('\t%f', maxROI);
param.ehcA = param.k_a * (1 - maxROI); % param.ehcA becomes 0 if maxROI == 1, resulting in deltaW being a matrix of 0 => no change in W

% get weights
maskPyra = gauss_pyra(mask);
[frame_boosted, deltaW] = boost_HSI(frame_enhanced, diffs, maskPyra);
% % remove the extra boosting, temporarily, to see how statistics
% change
% deltaW = zeros(size(W)); frame_out = frame_enhanced;

% updating of W
W = W_orig + deltaW;
if any(isnan(W(:)))
    error('NaN in weight matrix');
end

switch minim_opt.area
    case MinimisationOption.A_ROI
        minim_area = mask_locations;
    case MinimisationOption.A_IMG
        minim_area = true(size(SAft));
    otherwise, exit('Unknown minimisation area option');
end

pyras_after_boosting = make_pyras(frame_boosted, lastPyrasAft);
modu_saliency_after_boosting = process_saliency(get_salimap(pyras_after_boosting));

% % Experiment with changing the order of the minimisation operations

frame_in = frame_boosted;
frame_other = frame_orig;
frame_type_within_frame = minim_within_frame(frame_in, frame_other,...
    minim_area, minim_opt.within_frame, lastPyrasAft, cur_modu_saliency,...
    modu_saliency_after_boosting);

frame_in = frame_type_within_frame;
frame_other = frame_prev_edited;
frame_out_type_temporal = minim_temporal(frame_in, frame_other,...
    minim_area, minim_opt.temporal, lastPyrasAft);
frame_out = frame_out_type_temporal;

% frame_in = frame_boosted;
% frame_other = frame_prev_edited;
% frame_out_type_temporal = minim_temporal(frame_in, frame_other,...
%     minim_area, minim_opt.temporal, lastPyrasAft);
% 
% 
% frame_in = frame_out_type_temporal;
% frame_other = frame_orig;
% frame_type_within_frame = minim_within_frame(frame_in, frame_other,...
%     minim_area, minim_opt.within_frame, lastPyrasAft, cur_modu_saliency,...
%     modu_saliency_after_boosting);
% frame_out = frame_type_within_frame;

frame_out_v = frame_out(:);

min_pix = min(frame_out_v);
max_pix = max(frame_out_v);
% TODO(zori) the values will get 'hedged' to [0, 1]; is that a problem?
num_out_of_range = sum(sum(frame_out_v < 0 | frame_out_v > 1));
if min_pix < 0 || max_pix > 1
    assert(num_out_of_range ~= 0)
    warning(['out-of-range modulated value(s) - min: ' num2str(min_pix) '; max: ' num2str(max_pix)]);
    param.frames_out_of_range_modulated_vals = param.frames_out_of_range_modulated_vals + 1;
    param.num_out_of_range_modulated_vals = param.num_out_of_range_modulated_vals + num_out_of_range;
end
end
