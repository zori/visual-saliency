function [ inhibitedMap ] = simulate( map )
%SIMULATE select the most salilent teature and inhibit the region around it
%   @author Tao

    global param;
    
    [argmaxY, argmaxX] = find(map == max(map(:)));
    
    inhibition = zeros(size(map));
    [X, Y] = meshgrid(1:size(map, 2), 1:size(map, 1));
    inhibition = (X - argmaxX).^2 + (Y - argmaxY).^2;
    inhibition(inhibition > param.foaSqrRadius) = param.foaSqrRadius;
    inhibition = inhibition - param.foaSqrRadius;
    inhibitedMap = max(map + inhibition / param.foaSqrRadius, 0);
    
end