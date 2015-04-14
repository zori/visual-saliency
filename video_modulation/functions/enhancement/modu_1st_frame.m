function [ edtFrm, deltaW ] = modu_1st_frame( srcFrm, diffs, mask, SBef )
%MODU_1ST_FRAME modulation of 1st frame, with updated W returned
%   @author Tao

    global param;
    
    % prepare ehcA
    SROI  = SBef(logical(mask));
    maxROI = max(SROI(:));
    fprintf('\t%f', maxROI);
    param.ehcA = param.k_a * (1 - maxROI);
    
    % get weights
    maskPyra = gauss_pyra(mask);
    [edtFrm, deltaW] = boost_HSI(srcFrm, diffs, maskPyra);

end
