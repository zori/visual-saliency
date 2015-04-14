function [ result ] = simple_n( map )
%SIMPLE_N Normalise a map to intensity [0,1]
%   @author Tao

	min_value = min(map(:));
	max_value = max(map(:));
	result = (map - min_value) / (max_value - min_value);

end