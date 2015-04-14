function [ mask ] = get_video_mask( BB )
%GET_VIDEO_MASK make a ROI mask for current frame on given BoundingBox
%   @author Tao

    global param;

    mask = zeros(param.resY, param.resX);
    mask(round(BB(2)): round(BB(4)), ...
         round(BB(1)): round(BB(3))) = 1;
    mask = imfilter(mask, param.maskBlurKernel);
    
end

