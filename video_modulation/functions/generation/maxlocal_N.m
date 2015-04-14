function [ result ] = maxlocal_N( map )
%N map normalisation operator
%   @author Tao

    % normalise to [0,1]
	map = simple_n(map);
    
    % get mean of peaks
    argMax = map > imdilate(map, [1 1 1; 1 0 1; 1 1 1]);
    m_dash = mean(map(argMax));
    amplitude = (1 - m_dash)^2;

    % multiply
    result = amplitude * map;

end