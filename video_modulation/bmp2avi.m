imageNames = dir(fullfile('dataset/video/bridge_close/','*.png'));

outputVideo = VideoWriter('bridge_close.avi');
outputVideo.FrameRate = 30;
open(outputVideo);

for ii = 1:300
    img = imread(fullfile('dataset/video/bridge_close/',imageNames(ii).name));
    writeVideo(outputVideo,img);
end

close(outputVideo);