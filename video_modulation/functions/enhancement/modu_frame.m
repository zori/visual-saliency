function [ edtFrm, W ] = modu_frame( srcFrm, diffs, mask, W, lastPyrasAft )
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao
    
    global param;

    % pre-enhance
    edtFrm = enhance(srcFrm, diffs, mask, W);
    pyrasAft = make_pyras(edtFrm, lastPyrasAft);
    SAft = simple_n(enlarge(get_salimap(pyrasAft)));
    
    % prepare ehcA
    SROI  = SAft(logical(mask));
    maxROI = max(SROI(:));
    fprintf('\t%f', maxROI);
    param.ehcA = param.k_a * (1 - maxROI);
    
    % get weights
    maskPyra = gauss_pyra(mask);
    [edtFrm, deltaW] = boost_HSI(edtFrm, diffs, maskPyra);
    
    % updating of W
    W = W + deltaW;

end
