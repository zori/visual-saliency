% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function varargout = process_saliency(varargin)
varargout = cell(size(varargin));
for k=1:nargin
    varargout{k} = simple_norm(enlarge(varargin{k}));
end
end
