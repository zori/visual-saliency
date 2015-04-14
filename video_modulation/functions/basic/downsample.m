function [ result ] = downsample( image )
%DOWNSAMPLE 2x downsample 
%   @author Tao

    result = imresize(image, 0.5);
	% result = imresize(image, 0.5, 'nearest');

end