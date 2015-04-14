function [ rgbImage ] = ihls2rgb( ihslImage )
%HSI2RGB covert image in IHLS color space to RGB space
%        H is in [0, 2*pi) and both S and L are in [0,1);
%   @author Tao 

    H = ihslImage(:,:,1);
    S = ihslImage(:,:,2);
    L = ihslImage(:,:,3);
    
    C = 0.866025403784439*S ./ sin(2.094395102393195 - mod(H, pi/3));
    C_1 =  C .* cos(H);
    C_2 = -C .* sin(H);
    
    rgbImage(:,:,1) = L + 2/3*C_1;
    rgbImage(:,:,2) = L - C_1/3 - 0.577350269189626*C_2;
    rgbImage(:,:,3) = L - C_1/3 + 0.577350269189626*C_2;

end