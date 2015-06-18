function [ S, S_flicker ] = get_salimap( pyras )
%GET_MAP Generate saliency map for each given frame
%   @author Tao

    global param;
    
    % for both image and video --------------------------------------------
        
    % difference maps
    diffs.I  = across_scale_diff(pyras.I);
    diffs.RG = across_scale_diff(pyras.R, pyras.G);
    diffs.BY = across_scale_diff(pyras.B, pyras.Y);
    diffs.O  = cell(param.gaborOrients, param.nFeatureMaps);
    for orientIdx = 1:param.gaborOrients
        [diffs.O(orientIdx, :)] = across_scale_diff(pyras.O(orientIdx, :));
    end
    
    % feature maps
    ftrMaps.I = N(diffs.I);
    ftrMaps.RG = N(diffs.RG);
    ftrMaps.BY = N(diffs.BY);
    ftrMaps.O  = cell(param.gaborOrients, param.nFeatureMaps);
    for orientIdx = 1:param.gaborOrients
        [ftrMaps.O(orientIdx, :)] = N(diffs.O(orientIdx, :));
    end

    % conspicuity maps
    % I
    cspMaps.I = across_scale_add(ftrMaps.I);
    % C
    ftrMaps.C = cell(param.nFeatureMaps, 1);
    for mapIdx = 1:param.nFeatureMaps
        ftrMaps.C{mapIdx} = ...
            abs(ftrMaps.RG{mapIdx}) + abs(ftrMaps.BY{mapIdx});
    end
    cspMaps.C = across_scale_add(ftrMaps.C);
    % O
    cspMaps.O = zeros(size(cspMaps.C));
    for orientIdx = 1:param.gaborOrients
        cspMaps.O = cspMaps.O+N(across_scale_add(ftrMaps.O(orientIdx,:)));
    end

    
    if isfield(pyras, 'F')
    % for video only ------------------------------------------------------

        % feature maps and conspicuity maps
        % flicker
        ftrMaps.F = N(across_scale_diff(pyras.F));
        cspMaps.F = across_scale_add(ftrMaps.F);
        % motion
        ftrMaps.M = cell(param.motionDirects, param.nFeatureMaps);
        cspMaps.M = zeros(size(cspMaps.C));
        for directIdx = 1:param.motionDirects
            ftrMaps.M(directIdx, :) = ...
                N(across_scale_diff(pyras.M(directIdx, :)));
            cspMaps.M = ...
                cspMaps.M + N(across_scale_add(ftrMaps.M(directIdx, :)));
        end
        
        % master saliency map
        S = (N(cspMaps.I) + N(cspMaps.C) + N(cspMaps.O) + ...
             N(cspMaps.M)) / 3; % divide by 3, as in 'Realistic avatar eye and head animation using a neurobiological model of visual attention'
        % flicker saliency
        S_flicker=cspMaps.F;
        
    else
    % for image only ------------------------------------------------------
    
        % master saliency map
        S = (N(cspMaps.I) + N(cspMaps.C) + N(cspMaps.O)) / 3;
        S_flicker = zeros(size(S));
    end
    % fprintf('mean saliency:%f\n',mean(S(:)))
end
