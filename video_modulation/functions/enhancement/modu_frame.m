function [ frame_out, W ] = modu_frame( frame_in, diffs, mask, W, lastPyrasAft )
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao
    
    global param;

    % pre-enhance
    frame_enhanced = enhance(frame_in, diffs, mask, W);
    pyrasAft = make_pyras(frame_enhanced, lastPyrasAft);
    SAft = simple_norm(enlarge(get_salimap(pyrasAft)));
    
    % mark the location of max saliency after the modulation
    % figure; im(SAft); [~, maxind] = max(SAft(:)); [ri,ci] = ind2sub([r, c], maxind); hold on; plot(ci, ri, 'x');
    % prepare ehcA
    SROI  = SAft(logical(mask));
    maxROI = max(SROI(:));
    param.modu_frame_calls = param.modu_frame_calls + 1;
    if maxROI ~= 1, param.maxROI_nz = param.maxROI_nz + 1; end
    fprintf('\t%f', maxROI);
    param.ehcA = param.k_a * (1 - maxROI); % param.ehcA becomes 0 if maxROI == 1, resulting in deltaW being a matrix of 0 => no change in W
    
    % get weights
    maskPyra = gauss_pyra(mask);
    [frame_out, deltaW] = boost_HSI(frame_enhanced, diffs, maskPyra);
    % TODO(zori) remove the extra boosting, temporarily, to see how statistics
    % change
    % deltaW = zeros(size(W)); frame_out = frame_enhanced;
    
    % updating of W
    W = W + deltaW;
end
