function wpsnr = WPSNR(iref,icoded,wt,h,w)
%--------------------------------------------------------------------------
% Function to compute a weighted PSNR
% Inputs:
%   iref: the original frame
%   icoded : the coded frame
%   wt : a weight matrix of the same size of iref and iq
%   h: the height of the frame
%   w: the width of the frame
% Outputs:
%   wpsnr: the output WPSNR value
%--------------------------------------------------------------------------
peak = log10(255^2);
seq = (iref-icoded).^2;   % the square error, iref is the reference frame, and iq is the compressed one, we only compute the Y component of the sequence
wmseq = w*h*mean2(wt .* seq);  % the weighted mean square error
wpsnr = 10 * (peak - log10(wmseq));  % the wpsnr
end