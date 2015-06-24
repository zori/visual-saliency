function [ w1, w2 ] = weight( map, mask )
%WEIGHT weighting for difference and enhcancement maps
%   @author Tao

    absMap = abs(map);
    argMax = absMap > imdilate(absMap, [1 1 1; 1 0 1; 1 1 1]);
    
    % roi peaks
    inPeaks  = absMap(argMax &   mask );
    if isempty(inPeaks)
        inPeaks = max(max(absMap.*mask));
    end
    % background peaks
    outPeaks = absMap(argMax & (~mask));
    if isempty(outPeaks)
        outPeaks = max(max(absMap.*(1-mask)));
    end
    m1 = mean(inPeaks);
    m2 = mean(outPeaks);
    sigma1 = std(inPeaks);
    sigma2 = std(outPeaks);
    
    w1 = sigma1 * exp(m1 - m2); % weight for roi
    w2 = sigma2 * exp(m2 - m1); % weight for background pixels (outside the roi)
        
end
