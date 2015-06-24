function [ result ] = maxlocal_N( map )
%N map normalisation operator into [0,amplitude]
%   @author Tao

    % normalise to [0,1]
	map = simple_norm(map);
    
    % get mean of peaks
    argMax = map > imdilate(map, [1 1 1; 1 0 1; 1 1 1]);
    % Zori(fix)
    maximal_values=map(argMax);
    if isempty(maximal_values)
        % assert(all(map(:) == map(1))); % not always; sometimes there are a few non-zero values, which are not "peaks"
        m_dash = map(1); % TODO(zori) maybe mean(map(:)) ?
    else
        m_dash = mean(map(argMax));
    end
    amplitude = (1 - m_dash)^2;

    % multiply
    result = amplitude * map;
end
