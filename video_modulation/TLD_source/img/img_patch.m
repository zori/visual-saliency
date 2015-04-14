% University of Surrey
%
% This file is part of TLD.
%


function patch = img_patch(img, bb, randomize,p_par)

if nargin == 4 && randomize > 0
    
    rand('state',randomize);
    randn('state',randomize);
    
    NOISE = p_par.noise;
    ANGLE = p_par.angle;
    SCALE = p_par.scale;
    SHIFT = p_par.shift;
    
    cp  = bb_center(bb)-1;
    Sh1 = [1 0 -cp(1); 0 1 -cp(2); 0 0 1];
    
    sca = 1-SCALE*(rand-0.5);
    Sca = diag([sca sca 1]);
    
    ang = 2*pi/360*ANGLE*(rand-0.5);
    ca = cos(ang);
    sa = sin(ang);
    Ang = [ca, -sa; sa, ca];
    Ang(end+1,end+1) = 1;
    
    shR  = SHIFT*bb_height(bb)*(rand-0.5);
    shC  = SHIFT*bb_width(bb)*(rand-0.5);
    Sh2 = [1 0 shC; 0 1 shR; 0 0 1];
    
    bbW = bb_width(bb)-1;
    bbH = bb_height(bb)-1;
    box = [-bbW/2 bbW/2 -bbH/2 bbH/2];
    
    H     = Sh2*Ang*Sca*Sh1;
    bbsize = bb_size(bb);
    patch = uint8(warp(img,inv(H),box) + NOISE*randn(bbsize(1),bbsize(2)));
    
    
else
    
    % All coordinates are integers
    if sum(round(bb)-bb)==0
        L = max([1 bb(1)]);
        T = max([1 bb(2)]);
        R = min([size(img,2) bb(3)]);
        B = min([size(img,1) bb(4)]);
        patch = img(T:B,L:R);
        
        % Sub-pixel accuracy
    else
        
        cp = 0.5 * [bb(1)+bb(3); bb(2)+bb(4)]-1;
        H = [1 0 -cp(1); 0 1 -cp(2); 0 0 1];
        
        bbW = bb(3,:)-bb(1,:);
        bbH = bb(4,:)-bb(2,:);
        if bbW <= 0 || bbH <= 0
            patch = [];
            return;
        end
        box = [-bbW/2 bbW/2 -bbH/2 bbH/2];
        
        if size(img,3) == 3
            for i = 1:3
                P = warp(img(:,:,i),inv(H),box);
                patch(:,:,i) = uint8(P);
            end
        else
            patch = warp(img,inv(H),box);
            patch = uint8(patch);
        end

    end
end


