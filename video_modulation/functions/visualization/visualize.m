function [ handle ] = visualize( crtFrame, editedFrame, SBef, SAft, ...
                                 W, BB, handle )
%VISUALIZE visualization
%   @author Tao

    figure(2);
    
    subplot(2,2,1);
    set(handle.imgBef, 'CData', crtFrame);
    set(handle.r1, 'Position', [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)]);
    
%     subplot(2,3,2);
%     set(handle.mapBef, 'CData', SBef);
%     set(handle.r2, 'Position', [BB(1) BB(2) BB(3)-BB(1) BB(4)-BB(2)]);
    
    subplot(2,2,2);
	bar(squeeze(W(1:6, [1 3 5]))', 'stacked');
    
    subplot(2,2,3);
    set(handle.imgAft, 'CData', editedFrame);
    
%     subplot(2,3,5);
%     set(handle.mapAft, 'CData', SAft);
    
    subplot(2,2,4);
	bar(squeeze(-W(1:6, [2 4 6]))', 'stacked');
    
    % subplot(2,2,[3 4]);
    % set(p(1),'XData', 1:i, 'YData', maxSal(1:i));
    % set(p(2),'XData', 1:i, 'YData', minSal(1:i));
    % set(p(3),'XData', 1:i, 'YData', tamplateSal(1:i));
    % axis([0 ceil((i+2)/10)*10 0 ceil(max(maxSal))]);
    
    drawnow;

end

