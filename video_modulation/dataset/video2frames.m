% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)
% the size for the images (we are resizing the videos to match our requirements)
numrows = 288;
numcols = 352;
imgs_format = '.png';

% videos for the new dataset
% input_video_name = 'baseball';
% input_video_name = 'beach';
% input_video_name = 'canal';
% input_video_name = 'newport';
% input_video_name = 'palace';
input_video_name = 'vegas';
input_video_format = '.mp4';
input_video = [input_video_name input_video_format];
readerObj = VideoReader(input_video);

imgs_dir = [input_video(1:end-4) '_' datestr(clock,'HH-MM-SS')];
disp(imgs_dir)
mkdir(imgs_dir);
% cd(imgs_dir);

k = 1;
% TODO(zori) actually, we only need 300 frames
while hasFrame(readerObj)
    % fprintf('%03d\n', k);
    img_name = fullfile(imgs_dir, [num2str(k, '%03d') imgs_format]);
    frame = readFrame(readerObj);
    frame_resized = imresize(frame, [numrows numcols]);
    imwrite(frame_resized, img_name, 'png');
    k = k + 1;
end
