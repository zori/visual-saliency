function [ pyramid ] = gabor_pyra( G_n )
%GABOR_PYRA Generate gabor pyramid 
%           with original output resolution on each level
%   @author Tao
%   adapted from Overcomplete Steerable Pyramid Filters 
%                and Rotation Invariance (Greenspan et.al 1994)
%
%   To be compatible with operation for gaussian pyramids, every scale here
%   is 1 scale lower than them in model proposed in the paper above
%   so scale 0 doesn't exist, and each other scale is at the same
%   resolution of the guassian pyramid.

	global param;
    pyramid = cell(param.gaborOrients, param.pyraScales);
    
%   this was used in original model
%     % process original image to scale 1
%     % to skip generation of the useless scale 0 gabor
%     image = downsample(imfilter(image, param.guassKernel, 'replicate'));

    % scale 0 has all black gabor images
    for orientIdx = 1:param.gaborOrients
        pyramid{orientIdx, 1} = zeros(param.resY, param.resX);
    end
    % from scale 1 up
	for scaleIdx = 2:(param.pyraScales+1)
        G_0 = imfilter(G_n, param.gaussKernel, 'symmetric');
        L_n = G_n - G_0;
        G_n = downsample(G_0);

        for orientIdx = 1:param.gaborOrients
            tempImage = ...
                imfilter(L_n, param.gaborPsy{orientIdx}, 'symmetric');
            pyramid{orientIdx, scaleIdx} = downsample(tempImage);
        end
        
    end         
end