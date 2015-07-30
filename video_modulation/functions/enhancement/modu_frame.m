function [ frame_out, W ] = modu_frame( frame_in, frame_ind, diffs, mask, W_orig, lastPyrasAft )
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao
    
    global param;

    % pre-enhance
    frame_enhanced = enhance(frame_in, diffs, mask, W_orig);
    pyrasAft = make_pyras(frame_enhanced, lastPyrasAft);
    SAft = simple_norm(enlarge(get_salimap(pyrasAft)));
    
    % mark the location of max saliency after the modulation
    % figure; im(SAft); [~, maxind] = max(SAft(:)); [ri,ci] = ind2sub([r, c], maxind); hold on; plot(ci, ri, 'x');
    % prepare ehcA
    mask_locations = mask ~= 0;
    SROI  = SAft(mask_locations);
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
%         figure(7); im(frame_in); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_f'], 'jpg');
%         figure(8); im(SAft); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_SAft'], 'jpg');
    end
    fprintf('\t%f', maxROI);
    param.ehcA = param.k_a * (1 - maxROI); % param.ehcA becomes 0 if maxROI == 1, resulting in deltaW being a matrix of 0 => no change in W
    
    % get weights
    maskPyra = gauss_pyra(mask);
    [frame_out, deltaW] = boost_HSI(frame_enhanced, diffs, maskPyra);
    % TODO(zori) remove the extra boosting, temporarily, to see how statistics
    % change
    % deltaW = zeros(size(W)); frame_out = frame_enhanced;
    
    % updating of W
    W = W_orig + deltaW;
    if any(isnan(W(:)))
        error('NaN in weight matrix');
    end
end
