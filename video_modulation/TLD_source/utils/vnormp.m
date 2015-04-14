% University of Surrey
%
% This file is part of TLD.
%

function out = vnormp(in,p)
% p-norm of columns

out = (sum(abs(in).^p,1)).^(1/p);
