function [ pyramid ] = gauss_pyra( image )
%GAUSS_PYRA Generate gaussian pyramid with original output resolution on each level
%   @author Tao

    global param;
    
    pyramid = {};
    % scale 0
    pyramid{1} = image;
    % from scale 1 up
    for scaleIdx = 2:(param.pyraScales+1)
        image = imfilter(image, param.gaussKernel, 'symmetric');
        image = downsample(image);
        pyramid{scaleIdx} = image;
    end

end