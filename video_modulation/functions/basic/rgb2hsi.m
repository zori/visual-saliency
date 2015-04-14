function [ hsiImage ] = rgb2hsi( rgbImage )
%RGB2HSI covert image in RGB color space to HSI space
%        where H is in [0, 2*pi) and both S and L are in [0,1);
%   @author Tao 

    %% input
    R = rgbImage(:,:,1);
	G = rgbImage(:,:,2);
	B = rgbImage(:,:,3);

    %% conversion
	H = real(acos( (R - G/2 - B/2) ./ sqrt((R-G).^2 + (R-B).*(G-B)) ));
    H(B>G) = 2*pi - H(B>G);
    H(isnan(H)) = 0;
    
	I = (R + G + B) / 3;
    
    m = min(R, min(G, B));
    S = 1 - m./I;
    S(isnan(S)) = 0;
    
    %% output
    hsiImage(:,:,1) = H;
    hsiImage(:,:,2) = S;
    hsiImage(:,:,3) = I;

end