% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)
function make_calibration_video()
calibration_frame = make_calibration_frame();

fps = 30;
n_seconds = 6;
n_frames = fps * n_seconds;
video_name = 'calibration';
video_extension = 'avi';

output_video = VideoWriter([video_name, '.', video_extension]);
output_video.FrameRate = fps;
open(output_video);

for k = 1:n_frames
    writeVideo(output_video, calibration_frame);
end

close(output_video);
end

function calibration_frame = make_calibration_frame()
% a grey background with black cross in the middle
numrows = 288;
numcols = 352;

cross_w = 10;
cross_h = 30;

channel = ones(numrows, numcols) ./ 2;

% vertical strip
rows_fst = (numrows - cross_h) / 2;
cols_snd = (numcols - cross_w) / 2;
channel(rows_fst:rows_fst + cross_h, cols_snd:cols_snd + cross_w) = 0;

% horizontal strip
rows_snd = (numrows - cross_w) / 2;
cols_fst = (numcols - cross_h) / 2;
channel(rows_snd:rows_snd + cross_w, cols_fst:cols_fst + cross_h) = 0;

% make an output RGB image
calibration_frame = repmat(channel, 1, 1, 3); % cat(3, channel, channel, channel);
% figure; im(calibration_frame);
end
