% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function [min_pix, max_pix, num_out_of_range, frame_out] = check_out_of_range_values(frame_out)
global param;
frame_out_v = frame_out(:);
min_pix = min(frame_out_v);
max_pix = max(frame_out_v);

num_out_of_range = sum(sum(frame_out_v < 0 | frame_out_v > 1));
if min_pix < 0 || max_pix > 1
    assert(num_out_of_range ~= 0)
    % warning(['out-of-range value(s) - min: ' num2str(min_pix) '; max: ' num2str(max_pix)]);

    param.frames_out_of_range_modulated_vals = param.frames_out_of_range_modulated_vals + 1;
    param.num_out_of_range_modulated_vals = param.num_out_of_range_modulated_vals + num_out_of_range;

    % hedge the output to [0, 1]
    frame_out(frame_out < 0) = 0;
    frame_out(frame_out > 1) = 1;
end
end
