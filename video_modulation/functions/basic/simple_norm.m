function [ result ] = simple_norm( map )
%SIMPLE_N Normalise a map to intensity [0,1]
%   @author Tao

	min_value = min(map(:));
	max_value = max(map(:));
    diff = max_value - min_value;
    avoid_zero_division_trick = diff + (diff == 0); % by Zori; adds 1 in case all values in map are the same
    result = (map - min_value) ./ avoid_zero_division_trick;
end
