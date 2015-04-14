function [ result ] = interpolate( image, targetScale )
%INTERPOLATE interpolate an image to wanted scale 
%            and wanted resolution is given by wantedSize
%   @author Tao
   
    global param;
    
    result = imresize(image, param.pyraRes{targetScale});
    % result = imresize(image, param.pyraRes{targetScale}, 'nearest');

end