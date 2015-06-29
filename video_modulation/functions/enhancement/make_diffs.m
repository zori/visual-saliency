function [ diffs ] = make_diffs( pyras )
%MAKE_DIFFS make diff maps
%   @author Tao

    global param;
    
    
    diffs = cell(param.nEhcMaps, param.N_DIFF_CHANNELS);
    diffs(:,1) = get_diffmaps(pyras.I);
    diffs(:,2) = get_diffmaps(pyras.R, pyras.G);
    diffs(:,3) = get_diffmaps(pyras.B, pyras.Y);
    
end
