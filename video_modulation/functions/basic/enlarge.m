function [ result ] = enlarge( map )
%ENLARGE (normalise map for visualisation and) resize map to original image size
%   @author Tao

	global param;
    
    result = imresize(map, [param.resY param.resX]);
    % result = imresize(map, [param.resY param.resX], 'nearest');
    
end