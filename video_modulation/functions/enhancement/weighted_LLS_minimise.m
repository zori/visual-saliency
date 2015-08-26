% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function p = weighted_LLS_minimise(weight, minim_area, A, b)
global param;
minim_area_weight = weight(minim_area);
assert(all(minim_area_weight) >= 0)
minim_area_weight = minim_area_weight / norm(minim_area_weight, 1);
FW = repmat(minim_area_weight, param.nChannel, 1);
% % the following creates a huge matrix
% DFW = diag(FW);
% p = (A' * DFW * A) \ (A' * DFW * b);
% % instead, precalculate: define AFW, such that AFW == A' * DFW
AFW = [A(:,1) .* FW FW]';
p = (AFW * A) \ (AFW * b);
end
