% University of Surrey
%
% This file is part of TLD.
%


function idx_out = bb_isout(bb,imsize)

idx_out = bb(1,:) > imsize(2) | ...
bb(2,:) > imsize(1) | ...
bb(3,:) < 1 | ...
bb(4,:) < 1;

