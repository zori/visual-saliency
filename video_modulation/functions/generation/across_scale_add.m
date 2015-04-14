function [ cspMap ] = across_scale_add( ftrMaps )
%ACROSS_SCALE_ADD add all given maps up at the coarsest scale
%   @author Tao

    global param;
    mapScale = param.mapScale;
    cspMap = zeros(param.pyraRes{mapScale+1});
    
    for mapIdx = 1:param.nFeatureMaps
        currentMap = ftrMaps{mapIdx};
        if param.c(mapIdx) < mapScale
            for scaleItr = 1:(mapScale - param.c(mapIdx))
                currentMap = ...
                    imfilter(currentMap, param.gaussKernel, 'symmetric');
                currentMap = downsample(currentMap);
            end
        elseif param.c(mapIdx) > mapScale
            currentMap = interpolate(currentMap, mapScale+1);
        end
        cspMap = cspMap + currentMap;
    end

end
