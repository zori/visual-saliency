function [ rgbImage ] = hsi2rgb( hsiImage )
%HSI2RGB covert image in HSI color space to RGB space
%        H is in [0, 2*pi) and both S and I are in [0,1);
%   @author Tao 
    
    %% input
    H = hsiImage(:,:,1);
    S = hsiImage(:,:,2);
    I = hsiImage(:,:,3);
    [resY, resX] = size(H);
    
    %% conversion
    case1 = (H<=pi*2/3);
    case3 = (H> pi*4/3);
    case2 = ~(case1 | case3);
    
    R = zeros(resY, resX);
    G = zeros(resY, resX);
    B = zeros(resY, resX);
    sinH = sin(H);
    cosH = cos(H);
    tanH = tan(H);
    
    B(case1) = I(case1) .* (1-S(case1));
    R(case1) = I(case1) .* (1 + ...
        (S(case1).*cos(H(case1))) ./ (cos(pi/3-H(case1))) );
    G(case1) = I(case1)*3 - (B(case1)+R(case1));
    
    H(case2) = H(case2) - pi*2/3;
    R(case2) = I(case2) .* (1-S(case2));
    G(case2) = I(case2) .* (1 + ...
        (S(case2).*cos(H(case2))) ./ (cos(pi/3-H(case2))) );
    B(case2) = I(case2)*3 - (R(case2)+G(case2));
    
    H(case3) = H(case3) - pi*4/3;
    G(case3) = I(case3) .* (1-S(case3));
    B(case3) = I(case3) .* (1 + ...
        (S(case3).*cos(H(case3))) ./ (cos(pi/3-H(case3))) );
    R(case3) = I(case3)*3 - (G(case3)+B(case3));
    
    %% correction for out of range velues
    % for R out of range
    outR = (R>1) & (R>=G) & (R>=B);
    R(outR) = 1;
    G(outR) =  sqrt(3)/2*(1 - I(outR)).*tanH(outR) + 1.5*I(outR) - 0.5;
    B(outR) = -sqrt(3)/2*(1 - I(outR)).*tanH(outR) + 1.5*I(outR) - 0.5;
    % for G out of range
    outG = (G>1) & (G>=B) & (G>=R);
    G(outG) = 1;
    B(outG) = 2*I(outG) + 2*(1-I(outG)).*cosH(outG)./(cosH(outG)-sqrt(3)*sinH(outG)) - 1;
    R(outG) = I(outG) - 2*(1-I(outG)).*cosH(outG)./(cosH(outG)-sqrt(3)*sinH(outG));
    % for B out of range
    outB = (B>1) & (B>=G) & (B>=R);
    B(outB) = 1;
    G(outB) = 2*I(outB) + 2*(1-I(outB)).*cosH(outB)./(cosH(outB)+sqrt(3)*sinH(outB)) - 1;
    R(outB) = I(outB) - 2*(1-I(outB)).*cosH(outB)./(cosH(outB)+sqrt(3)*sinH(outB));
    
    %% output
    rgbImage(:,:,1) = min(max(R, 0), 1);
    rgbImage(:,:,2) = min(max(G, 0), 1);
    rgbImage(:,:,3) = min(max(B, 0), 1);
    
end

