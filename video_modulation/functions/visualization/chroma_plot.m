function [ ] = chroma_plot( rgbBef, hsiBef, rgbAft, hsiAft, mask )
%CHROMA_PLOT plot chromatic distribution on a 2D chromatic plane
%   @author Tao

    global param;
    
    %% parameters
    
    arcI = param.chroplot_arcI;
    nArcs = param.chroplot_nArcs;
    nImageDots = param.chroplot_nImageDots;
    nROIDots = param.chroplot_nROIDots;
    
    %% data preprocessing
    
    imgHueBef = reshape(hsiBef(:,:,1), [], 1); 
    imgSatBef = reshape(hsiBef(:,:,2), [], 1); 
    imgRgbBef = max(min(squeeze(reshape(rgbBef, [], 1, 3)), 1), 0); 
    imgHueAft = reshape(hsiAft(:,:,1), [], 1); 
    imgSatAft = reshape(hsiAft(:,:,2), [], 1);
    imgRgbAft = max(min(squeeze(reshape(rgbAft, [], 1, 3)), 1), 0);
    
    inside = logical(mask);
    outside = ~inside;
    
    %% inside the ROI
    subplot(1,2,1);
    
    hueBef = imgHueBef(inside);
    satBef = imgSatBef(inside);
    rgbBef = imgRgbBef(inside,:);
    hueAft = imgHueAft(inside);
    satAft = imgSatAft(inside);
    rgbAft = imgRgbAft(inside,:);
    
    polar(0, 0, '+k'); hold on;
    title('ROI pixels');
    arcHsi = ones(1, 1, 3);
    arcHsi(:,:,3) = 0.3;
    for arcHue = 0:2*pi/nArcs:2*pi
        arc = polar([arcHue - pi/nArcs, arcHue + pi/nArcs], [1, 1]);
        arcHsi(:,:,1) = arcHue;
        arcRgb = max(min(hsi2rgb(arcHsi),1),0);
        set(arc, 'Color', squeeze(arcRgb)', 'LineWidth', 2);
    end
    
    ROIStep = ceil(length(hueBef)/nROIDots);
    for pxIdx = 1:ROIStep:length(hueBef)
        l = polar([hueBef(pxIdx), hueAft(pxIdx)], ...
                  [satBef(pxIdx), satAft(pxIdx)]);
        set(l, 'Color', rgbBef(pxIdx,:));
    end
    for pxIdx = 1:ROIStep:length(hueBef)
        p = polar(hueAft(pxIdx), satAft(pxIdx), '.');
        set(p, 'Color', rgbAft(pxIdx,:), 'markersize', 8);
    end
    
    %set(gca, 'Units', 'normalized', 'Position', [0 0 1 1]);
    
    %% outside the ROI
    subplot(1,2,2);
    
    hueBef = imgHueBef(outside);
    satBef = imgSatBef(outside);
    rgbBef = imgRgbBef(outside,:);
    hueAft = imgHueAft(outside);
    satAft = imgSatAft(outside);
    rgbAft = imgRgbAft(outside,:);
    
    polar(0, 0, '+k'); hold on;
    title('Other pixels');
    arcHsi = ones(1, 1, 3);
    arcHsi(:,:,3) = arcI;
    for arcHue = 0:2*pi/nArcs:2*pi
        arc = polar([arcHue - pi/nArcs, arcHue + pi/nArcs], [1, 1]);
        arcHsi(:,:,1) = arcHue;
        arcRgb = max(min(hsi2rgb(arcHsi),1),0);
        set(arc, 'Color', squeeze(arcRgb)', 'LineWidth', 2);
    end
    
    imageStep = ceil(length(hueBef)/nImageDots);
    for pxIdx = 1:imageStep:length(hueBef)
        l = polar([hueBef(pxIdx), hueAft(pxIdx)], ...
                  [satBef(pxIdx), satAft(pxIdx)]);
        set(l, 'Color', rgbBef(pxIdx,:));
    end
    for pxIdx = 1:imageStep:length(hueBef)
        p = polar(hueAft(pxIdx), satAft(pxIdx), '.');
        set(p, 'Color', rgbAft(pxIdx,:), 'markersize', 8);
    end
    
    %set(gca, 'Units', 'normalized', 'Position', [0 0 1 1]);
    
end