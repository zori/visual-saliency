%% image index controlling
startIdx = 1;
nImage = 50;

%% param
global param;
param = {};
nParamChoices = 1;
param.ehcA  = 1/10;
% ehcAOptions  = {1/50, 1/20, 1/10, 1/5, 1/2};
param.ehcKc = 5;
% ehcKcOptions = {2, 3, 5, 10, 20};
param.ehcKd = 10;
% ehcKdOptions = {2, 3, 5, 10, 20};
param.moduC = [1, 2, 3];
param.moduDelta = [3, 4];

%% result computation
% matrix preparation
nIterMat = zeros(nParamChoices, nImage);
iDiffMat = zeros(nParamChoices, nImage);
meanDiff = zeros(nParamChoices,1);
stdDiff  = zeros(nParamChoices,1);
meanIter = zeros(nParamChoices,1);
stdIter  = zeros(nParamChoices,1);

% giant loop
for paramIdx = 1:nParamChoices
    
    imageFileNum = startIdx;
    fprintf('\n\nparam option %d ...\n\n', paramIdx);
    
    for imageIdx = 1:nImage

        % print text
        fprintf('modulating image no.%d ...\n', imageFileNum);
        % import images
        sourceImage = im2double(imread( ...
                        strcat(num2str(imageFileNum, '%02d'),'.jpg') ...
                      ));
        mask = im2double(imread( ...
                 strcat(num2str(imageFileNum, '%02d'),'_M.bmp') ...
               ));
        % select and set param
        param.ehcA  = ehcAOptions{paramIdx};
        param.ehcKc = ehcKcOptions{paramIdx};
        param.ehcKd = ehcKdOptions{paramIdx};
        [param.resY, param.resX, param.nChannel] = size(sourceImage);
        set_param;
        
        % woo~ big boss
        saliency_image;
        % woo~
        
        % print and save
        fprintf('completed in %d iteration(s)\n', nIter);
        nIterMat(paramIdx, imageIdx) = nIter;
        fprintf('pixel difference is: %f\n', imageDiff);
        iDiffMat(paramIdx, imageIdx) = imageDiff;
        % index increment
        imageFileNum = imageFileNum + 1;

    end
    
    % postprocess result
    meanDiff(paramIdx) = mean(iDiffMat(paramIdx, :));
    stdDiff(paramIdx)  = std(iDiffMat(paramIdx, :)); 
    meanIter(paramIdx) = mean(nIterMat(paramIdx, :));
    stdIter(paramIdx)  = std(nIterMat(paramIdx, :));
    
end
