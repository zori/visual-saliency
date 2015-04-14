% University of Surrey
%
% This file is part of TLD.
%


function flag = bb_isin(bb,imsize)

flag = bb(1,:) >= 1 & ...
bb(2,:) >= 1 & ...
bb(3,:) <= imsize(2) & ...
bb(4,:) <= imsize(1);
