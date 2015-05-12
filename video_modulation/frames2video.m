video_sequence = 'etdb_foreman_352x288_30'; % 'bridge_close'
video_folder = fullfile('dataset', 'video', video_sequence);
img_extension = 'png';
video_extension = 'avi';
n_frames = 300;

imgs = dir(fullfile(video_folder, filesep, ['*.' img_extension]));

output_video = VideoWriter([video_folder, '.', video_extension]);
output_video.FrameRate = 30;
open(output_video);

for k = 1:n_frames
    img = imread(fullfile(video_folder, imgs(k).name));
    writeVideo(output_video, img);
end

close(output_video);
