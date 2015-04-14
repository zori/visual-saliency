% University of Surrey
%
% This file is part of TLD.
%

function idx = pseudorandom_indexes(N,k)

start = randi(k,1,1);

idx = start:k:N;
