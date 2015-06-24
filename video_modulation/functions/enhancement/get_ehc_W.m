function [ ehc, wMat ] = get_ehc_W( diff, maskPyra )
%GET_ENHANCE produce an enhacement map from given feature maps
%            within the grayscale mask
% INPUTS
%  diff                  - [param.nEhcMaps x 1] cell of pyramid differences
%  maskPyra              - [1 x param.pyraScales + 1] Gaussian pyramid of the
%                          roi mask (+ 1 for the original mask)
%
% OUTPUTS
%  ehc                  - [param.resY x param.resX] the enhanced (greyscale) image
%  wMat                 - [param.nEhcMaps x 2] enhancement weights
%                         first column: roi pixels' weights
%                         second column: backround pixels' weights
%
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
%     % TODO(zori) how much should this parameter be?
%     param.ehcA  = 1/10; % from experiment.m
%     param.ehcA = 1/3; % commented-out; from set_param.m
    ehc = param.ehcA * (roi - param.ehcBd * bkg);
    roi_bkg_M_matrix = [1/roiM, 0               ; ...
                                0     , param.ehcBd/bkgM];
    wMat = wMat * param.ehcA * roi_bkg_M_matrix;
end
