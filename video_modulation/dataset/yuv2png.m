% Zornitsa Kostadinova
% Apr 2015
% 8.5.0.197613 (R2015a)
function yuv2png()
% Used to convert the yuv dataset from SFU (ETDB) to individual .png frames.
% Frames is what the video modulation code (written by Tao SHI) uses.
video_path=fullfile('dataset', 'video');
% video_sequence='FOREMAN_352x288_30_orig.yuv';
video_sequence='mother-daughter_cif.yuv'; % 'flower_cif.yuv';
video_name=fullfile(video_path,'SFU_etdb','RAW',video_sequence);
output_folder='etdb_foreman_352x288_30';
output_path=fullfile(video_path,output_folder);

W = 352;
H = 288;
n_frames=300;
for k=1:n_frames
    % Read one frame from the input YUV file and display it
    frame = loadFileYuv(video_name,W,H,k);
    imshow(frame);
    % imwrite(frame,fullfile(output_path,sprintf('%3.3d.png',k)));
end
end
