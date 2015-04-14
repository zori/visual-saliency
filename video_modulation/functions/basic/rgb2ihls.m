function [ ihslImage ] = rgb2ihls( rgbImage )
%RGB2HSI covert image in RGB color space to IHLS space
%        where H is in [0, 2*pi) and both S and L are in [0,1);
%   @author Tao 

    R = rgbImage(:,:,1);
	G = rgbImage(:,:,2);
	B = rgbImage(:,:,3);

	H = real(acos( (R - G/2 - B/2) ./ sqrt((R-G).^2 + (R-B).*(G-B)) ));
    H(B>G) = 2*pi - H(B>G);
    H(isnan(H)) = 0;
	m = min(R, min(G, B));
    M = max(R, max(G, B));
    S = M - m;
	L = (R + G + B) / 3;
    
    ihslImage(:,:,1) = H;
    ihslImage(:,:,2) = S;
    ihslImage(:,:,3) = L;

end