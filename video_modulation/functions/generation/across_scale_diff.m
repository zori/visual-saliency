function [ diff ] = across_scale_diff( varargin )
%ACROSS_SCALE_DIFF across-scale difference (absoluted)
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
    
    diff = cell(param.nFeatureMaps, 1);
    for mapIdx = 1:param.nFeatureMaps
        % interpolate subtrahend in scale s to scale c
        interpSubtrahend = ...
            interpolate(subtrahend{param.s(mapIdx)+1}, param.c(mapIdx)+1);
        diff{mapIdx} = abs(minuend{param.c(mapIdx)+1} - interpSubtrahend);
    end
    
end