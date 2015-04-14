function [ frm ] = enhance( frm, diffs, mask, W )
%ENHANCE enhance frame based on given weights
%   @author Tao

    global param;

    % difference maps weighted combination
    diffCell = cell(param.nEhcMaps, 6);
    diffCell(:, [1 3 5]) = diffs;
    diffCell(:, [2 4 6]) = diffs;
    weightedDiff = ...
        cellfun(@enlarge, cellfun(@(x,y) (x*y), diffCell, num2cell(W), ...
                                  'UniformOutput', false), ...
                'UniformOutput', false);
    weightedEhc = squeeze(sum(cell2mat(shiftdim(weightedDiff,-2)),3));
    invMask = 1 - mask;
    ehcI  = weightedEhc(:,:,1).*mask - weightedEhc(:,:,2).*invMask;
    ehcRG = weightedEhc(:,:,3).*mask - weightedEhc(:,:,4).*invMask;
    ehcBY = weightedEhc(:,:,5).*mask - weightedEhc(:,:,6).*invMask;
    [ehcH, ehcS] = RGBY2pol(ehcRG, ehcBY);

    % vector adddtion
    hsi = rgb2hsi(frm);
    [befX, befY] = pol2cart(hsi(:,:,1), hsi(:,:,2));
    [ehcX, ehcY] = pol2cart(ehcH, ehcS);
    [aftHue, aftSat] = cart2pol(befX + ehcX, befY + ehcY);
    aftInt = hsi(:,:,3) + ehcI;

    % output
    hsi(:,:,1) = mod(aftHue, 2*pi);
    hsi(:,:,2) = max(min(aftSat, 1), 0);
    hsi(:,:,3) = max(min(aftInt, 1), 0);
    frm = hsi2rgb(hsi);      
    
end
