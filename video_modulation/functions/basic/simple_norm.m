function [ result ] = simple_norm( map )
%SIMPLE_N Normalise a map to intensity [0,1]
%   @author Tao

	min_value = min(map(:));
	max_value = max(map(:));
    diff = max_value - min_value;
    result = safe_divide(map - min_value, diff);
end
