function [ hue, sat ] = RGBY2pol( RG, BY )
%RGBY2POL produce hue and saturation enhancement maps 
%   @author Tao

    [hue, sat] = cart2pol( RG*sqrt(3)/2, -BY + abs(RG)/2);
    hue = mod(hue - pi/6, 2*pi); % angle correction

end