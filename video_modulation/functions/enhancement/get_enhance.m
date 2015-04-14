function [ ehc, wMat ] = get_enhance( diff, maskPyra )
%GET_ENHANCE produce an enhacement map from given feature maps
%            within the grayscale mask
%   @author Tao

    global param;
    
    % difference summation
    roi = zeros(param.resY, param.resX);
    bkg = zeros(param.resY, param.resX);
    wMat = zeros(param.nEhcMaps, 2);
    
    for mapIdx = 1:param.nEhcMaps
        
        [map, M] = range_compress(diff{mapIdx});
        currentMask = maskPyra{param.modu_c(mapIdx) + 1};
         
        [w1, w2] = weight(map, logical(currentMask));
        roi = roi + enlarge(w1 * map);
        bkg = bkg + enlarge(w2 * map);
        wMat(mapIdx, :) = [w1/M, w2/M];
        
    end
    
    [roi, roiM] = range_compress(roi .*    maskPyra{1} );
    [bkg, bkgM] = range_compress(bkg .* (1-maskPyra{1}));
    ehc = param.ehcA * (roi - param.ehcBd * bkg);
    wMat = wMat * param.ehcA * [1/roiM, 0               ; ...
                                0     , param.ehcBd/bkgM];
end
