% Zornitsa Kostadinova
% May 2015
% 8.5.0.197613 (R2015a)
function single_frames2video(video_folder, n_frames)
% for example:
% video_folder = 'dataset/video/orig_city_352x288_30_300'
% n_frames = 300
img_extension = 'png';
video_extension = 'avi';

imgs = dir(fullfile(video_folder, filesep, ['*.' img_extension]));

output_video = VideoWriter([video_folder, '.', video_extension]);
output_video.FrameRate = 30;
open(output_video);

for k = 1:n_frames
    img = imread(fullfile(video_folder, imgs(k).name));
    writeVideo(output_video, img);
end

close(output_video);
end
