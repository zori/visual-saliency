function [ editedImg, dW ] = boost_HSI( img, diffs, maskPyra )
%BOOST_HSI boost saliency in HSI space according to enhancement maps 
%   @author Tao

    global param;
    
    hsi = rgb2hsi(img);
    
    % ehc maps
    [ehc.I,   IwMat] = get_enhance(diffs(:,1), maskPyra);
    [ehc.RG, RGwMat] = get_enhance(diffs(:,2), maskPyra);
    [ehc.BY, BYwMat] = get_enhance(diffs(:,3), maskPyra);
    [ehc.H , ehc.S] = RGBY2pol(ehc.RG * param.ehcBc, ehc.BY * param.ehcBc);
    dW = [IwMat, RGwMat * param.ehcBc, BYwMat * param.ehcBc];
    
    % vector addition
    [befX, befY] = pol2cart(hsi(:,:,1), hsi(:,:,2));
    [ehcX, ehcY] = pol2cart(ehc.H, ehc.S);
    [aftHue, aftSat] = cart2pol(befX + ehcX, befY + ehcY);
    aftInt = hsi(:,:,3) + ehc.I;
    
    % output
    hsi(:,:,1) = mod(aftHue, 2*pi);
    hsi(:,:,2) = max(min(aftSat, 1), 0);
    hsi(:,:,3) = max(min(aftInt, 1), 0);
    editedImg = hsi2rgb(hsi);

end
