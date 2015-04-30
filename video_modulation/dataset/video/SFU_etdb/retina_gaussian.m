function w = retina_gaussian (x,y,x_e,y_e,sigma_x,sigma_y)
%--------------------------------------------------------------------------
% Function to compute a 2D Gaussian weight
% Inputs: 
%   (x,y): the target location
%   (x_e,y_e): the fixation location
%   (sigma_x,sigma_y): the width of the 2D Gaussian along x and y directions
% Outputs:
%   w : the weight at location (x,y)
% Written by Hadi Hadizadeh, Oct. 2010
%--------------------------------------------------------------------------
w = (1/(2*pi*sigma_x*sigma_y))*exp(-((x-x_e)^2/(2*sigma_x^2)+(y-y_e)^2/(2*sigma_y^2)));
end