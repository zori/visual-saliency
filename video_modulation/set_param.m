function [ ] = set_param( )
%SET_PARAM All parameter assignments
%   @author Tao

    global param;

    %% video properties
    
    param.fps = 30;
    param.nFrames = 300;
    
    %% saliency map generation

    % pyramids
    param.pyraScales = 8;
    param.pyraRes = {};
    param.pyraRes{1} = [param.resY, param.resX];
    for scaleIdx = 2:(param.pyraScales+1)
        param.pyraRes{scaleIdx} = ceil(param.pyraRes{scaleIdx-1} / 2);
    end
    % gaussian pyra.
    param.gaussKernel = [1/16; 1/4; 3/8; 1/4; 1/16] * ...
                        [1/16, 1/4, 3/8, 1/4, 1/16];
    % gabor pyra.
    % accoring to Overcomplete Steerable Pyramid Filters ...
    % and Rotation Invariance (Greenspan et.al 1994)
    % the positive direction of axies are:
    %
    %          y|(90 degree)
    % (180      |
    %  degree)  |
    %     ------ ------ x (0 degree)
    %           | 
    %           |
    %           |
    %
    % therefore, the y axis is inverse to the image coordinate
    param.gaborOrients = 4;
    [x, y] = meshgrid(-2:2, 2:-1:-2);
    m = {};
    m{1} = cos(.5 * pi * x);
    m{2} = cos(.5 * pi / sqrt(2) * (x + y));
    m{3} = cos(.5 * pi * y);
    m{4} = cos(.5 * pi / sqrt(2) * (y - x));
    param.gaborPsy = {};
    for orientIdx = 1:param.gaborOrients
        param.gaborPsy{orientIdx} = log_gabor_filter(x, y, m{orientIdx});
    end
    % motion pyra.
    param.motionDirects = 4;
    param.motionVectors = {[0 1], [-1 1], [-1 0], [-1 -1]};
    
    % across-scale difference
    c = [2, 3, 4];
    delta = [3, 4];
    param.mapScale = 4;
    param.nFeatureMaps = length(c) * length(delta);
    [delta_grid, c_grid] = meshgrid(delta, c);
    param.c = reshape(c_grid, 1, param.nFeatureMaps);
    param.delta = reshape(delta_grid, 1, param.nFeatureMaps);
    param.s = param.c + param.delta;
    
    % iterative normalisation
    param.nIter = 10;
    param.c_inh = 0.02;
    dogHalfSize = 7;
    dogDelta = 4;
    dogK = 5;
    [kernelY, kernelX] = ...
        meshgrid(-dogHalfSize : dogHalfSize, -dogHalfSize : dogHalfSize);
    circKernel = kernelX.^2 + kernelY.^2;
    param.dogKernel = ...
        normpdf_z(circKernel, 0, dogDelta) - ...
        normpdf_z(circKernel, 0, dogK * dogDelta);
    
    %% fixation simulation
%     param.foaSqrRadius = 25;
 
    %% saliency boosting
    
    % mask refinement
	param.maskGradWidth = 3;
    param.maskBlurKernel = fspecial('average', param.maskGradWidth * 2);
    
    % across-scale difference
	param.moduC  = [0, 1, 2];
	param.moduDelta = [3, 4];
    param.nEhcMaps = length(param.moduC) * length(param.moduDelta);
    [modu_delta_grid,modu_c_grid] = meshgrid(param.moduDelta,param.moduC);
    param.modu_c = reshape(modu_c_grid, 1, param.nEhcMaps);
    param.modu_delta = reshape(modu_delta_grid, 1, param.nEhcMaps);
    param.modu_s = param.modu_c + param.modu_delta;
    
%     % for still image only
%     param.ehcA = 1/3;
%     param.ehcKc = 5;
%     param.ehcKd = 10;
    
    % run-time params
    param.k_a = 1/10;
    param.k_c = 1/2;
    param.wSpan = 9;
    param.flashL = ceil(param.wSpan / 2);
    
    % chromatic plot
    param.chroplot_arcI = 0.3;
    param.chroplot_nArcs = 180;
    param.chroplot_nImageDots = 50;
    param.chroplot_nROIDots = 50;
    
end
