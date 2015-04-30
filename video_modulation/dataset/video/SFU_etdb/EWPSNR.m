function ewpsnr = EWPSNR(file1, file2, x_e, y_e, sigma_x, sigma_y, width, height, FrameNo)
%--------------------------------------------------------------------------
% Function to compute Eyetracking-weighted PSNR (EWPSNR)
% Inputs:
%   file1 and file2 : input sequences to be compared in YUV 4:2:0 format
%   (x_e,y_e) : the fixation-point location
%   (sigma_x, sigma_y): fovea size, usually 32 or 64 pixels (for 2 degress of visual angle)
%   (width, height): the width and height of the video frames
%   FrameNo: The frame index for computing the EWPSNR
%
% Output: The EWPSNR value in dB of the specified frame
%
% Note: This function calss retina_gaussian.m and WPSNR.m
%
% Written by Hadi Hadizadeh (hha54@sfu.ca), October 2010
%--------------------------------------------------------------------------

%Compute the 2-D Gaussian weight around the fixation point
w = zeros(height,width);
for y=1:height
    for x=1:width
        w(y,x)=retina_gaussian(x,y,x_e,y_e,sigma_x,sigma_y);
    end
end

fin1 = fopen(file1,'r');
fin2 = fopen(file2,'r');
frmSize = 6*(width*height)/4;

fseek(fin1,(FrameNo-1)*frmSize,'bof');
fseek(fin2,(FrameNo-1)*frmSize,'bof');
ewpsnr=0;
frm1 = fread(fin1,frmSize,'uchar');
frm2 = fread(fin2,frmSize,'uchar');
if(feof(fin2))
    disp 'Error: Frame Not found for meausring the PSNR...'
    ewpsnr=0;
    break
end
Y1 = frm1(1:width*height);
y1 = reshape(Y1,[width height])';
Y2 = frm2(1:width*height);
y2 = reshape(Y2,[width height])';
%normalize the Gaussian weights
w = ones(height,width)/(height*width);

%storing the EWPSNR value of the current frame
ewpsnr = WPSNR(y1,y2,w,height,width);
fclose(fin1);
fclose(fin2);
end

