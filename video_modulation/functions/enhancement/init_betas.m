function [ bc, bd ] = init_betas( img, mask )
%INIT_BETAS initialise coefficient \beta_c, \beta_d
%   @author Tao
    
    global param

    hsi = rgb2hsi(img);
    imageSat = hsi(:,:,2);
    imageItn = hsi(:,:,3);
    satArgmax = imageSat > imdilate(imageSat, [1 1 1; 1 0 1; 1 1 1]);
    inSat  = imageSat(satArgmax &  logical(mask));
    outSat = imageSat(satArgmax & ~logical(mask));
    inItn  = imageItn(logical(mask));
    outItn = imageItn(~logical(mask));
    bc = ( mean(inSat(:)) / mean(outSat(:)) )^2 ...
            * sqrt( mean(inItn(:)) / mean(outItn(:)) ) * param.k_c;
        
    bd = nthroot((mean(mask(:))/(1-mean(mask(:)))), 4 );

end
