function [ pyras ] = make_pyras( varargin )
% MAKE_PYRAS generate pyramids at multiple scales:
% I, R, G, B, Y, - Gaussian pyramids,
% O, - Gabor pyramids for multiple orientations orientation,
% F, M (optional, if the lastPyras was an input argument) - flicker and motion,
% according to Reichardt model.
%   @author Tao

    % decide whether to process single image or sequence
    if nargin == 1
        image = varargin{1};
    else
        image = varargin{1};
        lastPyras = varargin{2};
    end

    % preprocessing
    I = (image(:, :, 1) + image(:, :, 2) + image(:, :, 3)) / 3;
    maxI = max(I(:));
    low_values = I < (maxI / 10);
    
    r = image(:, :, 1) ./ I;
    r(low_values) = 0;
    
    g = image(:, :, 2) ./ I;
    g(low_values) = 0;
    
    b = image(:, :, 3) ./ I;
    b(low_values) = 0;

    R = max(r - (g + b) / 2, 0);
    G = max(g - (r + b) / 2, 0);
    B = max(b - (r + g) / 2, 0);
    Y = max((r + g)/2 - abs(r - g)/2 - b, 0);
    
    % pyramids
    pyras.R = gauss_pyra(R); % red
    pyras.G = gauss_pyra(G); % green
    pyras.B = gauss_pyra(B); % blue
    pyras.Y = gauss_pyra(Y); % yellow
    pyras.I = gauss_pyra(I); % intensity
    pyras.O = gabor_pyra(I); % orientation
    pyras.S = shift_pyra(pyras.O); % shifted orientations, to be used in .F and .M
    
    if nargin > 1
        pyras.F = gauss_pyra(I - lastPyras.I{1}); % flicker
        pyras.M = cellfun(@(a,b,c,d) (abs(a.*b - c.*d)), ... % motion
                          pyras.O, lastPyras.S, lastPyras.O, pyras.S, ...
                          'UniformOutput', false);
    end

end

