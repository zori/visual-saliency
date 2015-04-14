function [ diff ] = get_diffmaps( varargin )
%GET_DIFFMAPS generate difference maps (not absoluted)
%   @author Tao
    
    global param;

    if nargin == 1
        minuend = varargin{1};
        subtrahend = varargin{1};
    elseif nargin == 2
        minuend = cell(param.pyraScales+1, 1);
        subtrahend = cell(param.pyraScales+1, 1);
        for scaleIdx = 1:(param.pyraScales+1);
            minuend{scaleIdx} = ...
                varargin{1}{scaleIdx} - varargin{2}{scaleIdx};
            subtrahend{scaleIdx} = ...
                varargin{2}{scaleIdx} - varargin{1}{scaleIdx};
        end
    end
    
    
    diff = cell(param.nEhcMaps, 1);
    for mapIdx = 1:param.nEhcMaps
        % interpolate subtrahend in scale s to scale c
        interpSubtrahend = ...
            interpolate(subtrahend{param.modu_s(mapIdx)+1}, ...
                        param.modu_c(mapIdx)+1);
        diff{mapIdx} = minuend{param.modu_c(mapIdx)+1} - interpSubtrahend;
    end
    
end