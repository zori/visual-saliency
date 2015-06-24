function [ frame_out, deltaW ] = boost_HSI( frame_in, diffs, maskPyra )
%BOOST_HSI boost saliency in HSI space according to enhancement maps 
%   @author Tao

    global param;
    
    % ehc maps
    [ehc.I,   IwMat] = get_ehc_W(diffs(:,1), maskPyra);
    [ehc.RG, RGwMat] = get_ehc_W(diffs(:,2), maskPyra);
    [ehc.BY, BYwMat] = get_ehc_W(diffs(:,3), maskPyra);
    [ehc.H , ehc.S] = RGBY2pol(ehc.RG * param.ehcBc, ehc.BY * param.ehcBc);
    deltaW = [IwMat, RGwMat * param.ehcBc, BYwMat * param.ehcBc];
    
    % TODO(zori) compare with enhance.m
    % vector addition
    hsi = rgb2hsi(frame_in);
    [befX, befY] = pol2cart(hsi(:,:,1), hsi(:,:,2));
    [ehcX, ehcY] = pol2cart(ehc.H, ehc.S);
    [aftHue, aftSat] = cart2pol(befX + ehcX, befY + ehcY);
    aftInt = hsi(:,:,3) + ehc.I;
    
    % output
    % TODO(zori) out-of-range values get cropped; is that ok?
    hsi(:,:,1) = mod(aftHue, 2*pi);
    hsi(:,:,2) = max(min(aftSat, 1), 0);
    hsi(:,:,3) = max(min(aftInt, 1), 0);
    frame_out = hsi2rgb(hsi);

end
