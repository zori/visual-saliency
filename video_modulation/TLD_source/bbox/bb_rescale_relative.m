% University of Surrey
%
% This file is part of TLD.
%


function BB = bb_rescale_relative(BB,s)
% Change
BB = BB(1:4);
if length(s) == 1
    s = s*[1 1];
end
if isempty(BB), BB = []; return; end

s1 = 0.5*(s(1)-1)*bb_width(BB);
s2 = 0.5*(s(2)-1)*bb_height(BB);
BB = BB + [-s1; -s2; s1; s2];



