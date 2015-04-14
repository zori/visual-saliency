function [ psy ] = log_gabor_filter( x, y, m_k )
%LOG_GABOR_FILTER Generate the gabor filter kernel psy_k(x,y)
%                 when given the real part of m_k
%                 and correspnding range of x, y
%   @author Tao 
%   accoring to Overcomplete Steerable Pyramid Filters 
%               and Rotation Invariance (Greenspan et.al 1994)

    psy = exp(- .5 * (x.^2 + y.^2)) .* m_k / (2*pi);
    % divided by 2pi is removed because of the normalisation below
    
%     % normalise
%     min_value = min(min(psy));
%     max_value = max(max(psy));
%     psy = (psy - min_value) / (max_value - min_value);
    
end