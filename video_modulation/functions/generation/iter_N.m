function [ result ] = iter_N( M )
%ITER_N iterative normalisation
%   @author Tao

    global param;
    
    % normalise to [0,1], then M = 1
    M = simple_n(M);
    
    for iter = 1:param.nIter
       M = max(0, M + imfilter(M, param.dogKernel, 'symmetric') - param.c_inh);
       % M = max(0, M + imfilter(M, param.dogKernel) - param.c_inh);
    end
    result = M;
    
end