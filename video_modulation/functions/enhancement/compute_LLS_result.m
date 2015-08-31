% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function frame_out = compute_LLS_result(frame_in, minim_area, A, p)
global param;
% Theta = p(1);
% phi = p(2);
minim_out = A * p;
minim_out = reshape(minim_out, length(minim_out) ./ param.nChannel,...
    param.nChannel);
frame_minim = zeros(size(frame_in));
for c = 1:param.nChannel
    chan = frame_in(:, :, c);
    chan(minim_area) = minim_out(:, c);
    frame_minim(:, :, c) = chan;
end
frame_out = frame_minim;
% For minim_within_frame out-of-range values (i.e, smaller than 0 and larger 
% than 1) occur. Hedging those increases flicker saliency. So don't do it.
% [~, ~, ~, frame_out] = check_out_of_range_values(frame_out);

% this won't hedge out-of-range values, just records the statistics
check_out_of_range_values(frame_out);
end
