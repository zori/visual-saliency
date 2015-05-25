function [ result ] = N( varargin )
%N map normalisation operator
%   @author Tao

    % TODO(zori) result is not in [0,1], but in ???
    if iscell(varargin{1})
        imageSet = varargin{1};
    else
        imageSet = cell(1);
        imageSet{1} = varargin{1};
    end
    
    result = cell(length(imageSet), 1);
    for idx = 1:length(imageSet)
        % use max/local normalisation
        result{idx} = maxlocal_N(imageSet{idx});
        % use iterative normalisation
        % result{idx} = iter_N(imageSet{idx});
    end 

    if length(result) == 1
        result = result{1};
    end
    
end
