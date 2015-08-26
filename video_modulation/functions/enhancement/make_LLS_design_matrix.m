% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function [A, b] = make_LLS_design_matrix(frame_in, frame_other, minim_area)
% (Weighted) LLS minimisation to keeps appearance of pixels denoted by
% the mask in `minim_area' as similar as possible to `frame_other'.

% (weighted) LLS
% Want to solve: Ap = b for unknown parameters p = [Theta phi]';
% A is a n x 2 matrix with the modulated pixels
% b are the minim_area pixel 'observations' (as they are in frame_other)
global param;
b = []; A = [];  % TODO(zori) use ROI_n * param.nChannel to pre-allocate
for c = 1:param.nChannel
    chan = frame_other(:, :, c);
    b = [b; chan(minim_area)];
    chan = frame_in(:, :, c);
    A = [A; chan(minim_area)];
end
A = [A ones(length(A), 1)];
end
