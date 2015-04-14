function [ handle ] = init_visual( crtFrame, editedFrame, SBef, SAft, ...
                                   W, BB )
%INIT_VISUAL initial visualisation window
%   @author Tao

    figure(2);
    handle = {};
    
    subplot(2,2,1);
    handle.imgBef = imshow(crtFrame);
    set(gca,'NextPlot','add');
    handle.r1 = rectangle('Position', ...
        [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)], 'edgecolor', 'y');
               
%     subplot(2,3,2);
%     handle.mapBef = imshow(SBef);
%     set(gca,'NextPlot','add');
%     handle.r2 = rectangle('Position', ...
%         [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)], 'edgecolor', 'y');
               
    subplot(2,2,2);
	bar(squeeze(W(1:6, [1 3 5]))', 'stacked');
    
    subplot(2,2,3);
    handle.imgAft = imshow(editedFrame);
    
%     subplot(2,3,5);
%     handle.mapAft = imshow(SAft);
    
    subplot(2,2,4);
    bar(squeeze(-W(1:6, [2 4 6]))', 'stacked');

    % subplot(2,2,[3 4]) % for saliency graph
    % hold on;
    % p = plot(1, maxSal(1),'--r', 1, minSal(1),'--g', ...
    %          1, tamplateSal(1),'-s','MarkerSize',5);
    % legend(p, 'global maxium', 'global minium', 'tracking');
    % xlabel('Frame');
    % ylabel('Saliency');
    % axis([0 10 0 ceil(maxSal(1))]);
    % grid on;
    
% 	colormap('jet');
    drawnow;

end
