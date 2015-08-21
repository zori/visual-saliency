% Zornitsa Kostadinova
% Jul 2015
% 8.5.0.197613 (R2015a)
function [saliency, saliency_flicker, frame_avg, frame_avg_abs] = pyras2saliency(pyras, orig_saliency_flicker)
% Produces frame saliency statistic based on the current frame's pyramids.
%
% USAGE
%  [saliency, saliency_flicker, frame_avg] = pyras2saliency(pyras)
%  [saliency, saliency_flicker, frame_avg, frame_avg_abs] = pyras2saliency(pyras, orig_saliency_flicker)
%
% INPUTS
%  pyras                 - pyramids for the current frame
%  orig_saliency_flicker - (optional) the saliency flicker of the original
%                          frame, if this is the modulated one
%
% OUTPUTS
%  saliency              - [h x w] saliency map
%  saliency_flicker      - [h x w] flicker saliency map: absolute for the
%                          original frame; difference values for the modulated frame
%  frame_avg             - average flicker saliency: absolute for the original
%                          frame; relative to the original for the modulated
%                          one, indicating the percentage of added flicker
%                          (undesirable) through the modulation
%  frame_avg_abs         - absolute average flicker saliency for the modulated
%                          frame

[saliency, saliency_flicker] = get_salimap(pyras);
[saliency, saliency_flicker] = process_saliency(saliency, saliency_flicker);
% in case we have the modulated saliency, the original video's saliency_flicker
% is passed too; subtract, to only reason about the difference
if nargin == 2
    [~, ~, frame_avg_abs] = pyras2saliency(pyras);
    saliency_flicker_relative = saliency_flicker - orig_saliency_flicker;
    saliency_flicker_relative_abs_avg = mean(abs(saliency_flicker_relative(:)));
    orig_frame_avg = mean(orig_saliency_flicker(:));
    frame_avg = safe_divide(saliency_flicker_relative_abs_avg, orig_frame_avg);
    saliency_flicker = saliency_flicker_relative;
else
    assert(all(saliency_flicker(:) >= 0));
    frame_avg = mean(saliency_flicker(:));
end
end
