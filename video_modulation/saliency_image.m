%% file import
%  still image
    sourceImage = im2double(imread('03.jpg'));
    % sourceImage = im2double(imread('IMG_1340.jpg'));
    % sourceImage = im2double(imread('Sample_input.jpg'));
    % sourceImage = im2double(imread('shiyakusyo01_adj.jpeg'));
%  ROI creation
    mask = im2double(imread('03_M.bmp'));
    % mask = im2double(rgb2gray(imread('IMG_1340_mask.bmp')));
    % mask = im2double(rgb2gray(imread('Sample_input_mask.bmp')));
    % mask = im2double(rgb2gray(imread('shiyakusyo01_mask.bmp')));

%% set parameters
global param;
% TODO(zori) Why does he clean the param that he took the trouble to set in
% experiment?
param = {};
% BEGIN zori (copied from experiment.m)
param.ehcA  = 1/10;
% ehcAOptions  = {1/50, 1/20, 1/10, 1/5, 1/2};
param.ehcKc = 5;
% ehcKcOptions = {2, 3, 5, 10, 20};
param.ehcKd = 10;
% ehcKdOptions = {2, 3, 5, 10, 20};
param.moduC = [1, 2, 3];
param.moduDelta = [3, 4];
% END zori

[param.resY, param.resX, param.nChannel] = size(sourceImage);
set_param;

%% some bsic data verification
if param.nChannel < 3
   disp('Color image expected. Process terminated.');
   return;
elseif min(param.resY, param.resX) < 2^param.pyraScales
   disp('Minimium image size should be 2^(highgest scale of pyramid).');
   disp('Process terminated.');
   return;
end

%% saliency map generation for original image
pyras = make_pyras(sourceImage);
SBef = get_salimap(pyras);
SBef = simple_n(enlarge(SBef));

%% saliency boosting
%  refine mask

    % only for images set from Aiko
    % TODO(zori) OMG, this will ONLY work for one type of resolution; WHY?!?!
    mask = mask(1:256,1:512);
    if size(mask,3) == 3
        mask = rgb2gray(mask);
    end
    mask([1:2, param.resY-1:param.resY], :) = 0;
    mask(:, [1:2, param.resX-1:param.resX]) = 0;
    
SE = strel('disk', param.maskGradWidth);
mask = imdilate(mask, SE);
blurKernel = fspecial('average', param.maskGradWidth*2);
mask = imfilter(mask, blurKernel);
maskPyra = gauss_pyra(mask);

editedImage = sourceImage;
SAft = SBef;
SAftROI = SAft .* logical(mask);
nIter = 0;

[param.ehcBc, param.ehcBd] = init_betas(sourceImage, mask);

if max(SAftROI(:)) < 1

    % apply boosting
    diffs = make_diffs(pyras);
    editedImage = boost_HSI(editedImage, diffs, maskPyra);

    % re-generation of saliency map
    pyras = make_pyras(editedImage);
    SAft = simple_n(enlarge(get_salimap(pyras)));
    SAftROI = SAft .* logical(mask);

    param.ehcBc = (param.ehcBc*(param.ehcKc-1) + 1) / param.ehcKc;
    param.ehcBd = (param.ehcBd*(param.ehcKd-1) + 1) / param.ehcKd;
    nIter = nIter + 1;
    
end

%  data statisics
pixelDiff = abs(sourceImage - editedImage) * 255;
imageDiff = mean(pixelDiff(:));

%% visualisation
%  show image and map before and after 
subplot(2,2,1);
% sourceImage = multiply(sourceImage, S);
imshow(sourceImage, 'Border', 'tight');
title('Source');
%
subplot(2,2,2);
imshow(SBef, 'Border', 'tight');
colormap(jet);
title('Saliency Map');
%
subplot(2,2,3);
imshow(editedImage, 'Border', 'tight');
title('Modulated Image');
%
subplot(2,2,4);
imshow(SAft, 'Border', 'tight');
colormap(jet);
title('Modulated Map');

% %  show chromatic change
% figure;
% chroma_plot(sourceImage, hsiBef, editedImage, hsiAft, mask);
