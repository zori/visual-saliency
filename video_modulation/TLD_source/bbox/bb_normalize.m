% University of Surrey
%
% This file is part of TLD.
%


function bb2 = bb_normalize(bb1,bb0)
% Change
% Shifts and rescales bboxes in BB1 according to the first bbox in BB0.

W = bb_width( bb0(:,1)) / bb_width(bb1(:,1));
H = bb_height( bb0(:,1)) / bb_height(bb1(:,1));
SH = bb_center( bb0(:,1)) - bb_center(bb1(:,1));

bb2 = bb_rescale(bb_shift(bb1,SH),[W,H]);

