function [ result ] = multiply( image, map )
%MULTIPLY multiply the original image with enlarged saliency map 
%   @author Tao

    global param;
    
    map = 0.2 + map * 0.8;
    if ndims(image) == 3
        result = zeros(param.resY, param.resX, 3);
        for chaItr = 1:3
            result(:, :, chaItr) = image(:, :, chaItr) .* map;
        end
    else
        result = image .* map;
    end
    
end