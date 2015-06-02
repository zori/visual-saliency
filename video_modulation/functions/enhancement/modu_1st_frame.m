function [ frame_out, deltaW ] = modu_1st_frame( frame_in, diffs, mask, SBef )
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
    [frame_out, deltaW] = boost_HSI(frame_in, diffs, maskPyra);
end
