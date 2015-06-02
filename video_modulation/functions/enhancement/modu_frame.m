function [ frame_out, W ] = modu_frame( frame_in, diffs, mask, W, lastPyrasAft )
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao
    
    global param;

    % pre-enhance
    frame_enhanced = enhance(frame_in, diffs, mask, W);
    pyrasAft = make_pyras(frame_enhanced, lastPyrasAft);
    SAft = simple_n(enlarge(get_salimap(pyrasAft)));
    
    % prepare ehcA
    SROI  = SAft(logical(mask));
    maxROI = max(SROI(:));
    fprintf('\t%f', maxROI);
    param.ehcA = param.k_a * (1 - maxROI);
    
    % get weights
    maskPyra = gauss_pyra(mask);
    [frame_out, deltaW] = boost_HSI(frame_enhanced, diffs, maskPyra);
    
    % updating of W
    W = W + deltaW;
end
