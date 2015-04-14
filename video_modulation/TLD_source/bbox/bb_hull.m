% University of Surrey
%
% This file is part of TLD.
%


function bb = bb_hull(bb0)

bb=[min(bb0(1,:)); ...
    min(bb0(2,:)); ...
    max(bb0(3,:)); ...
    max(bb0(4,:))];
