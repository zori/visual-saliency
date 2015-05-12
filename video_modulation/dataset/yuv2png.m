% Zornitsa Kostadinova
% Apr 2015
% 8.5.0.197613 (R2015a)
function yuv2png()
% Used to convert the yuv dataset from SFU (ETDB) to individual .png frames.
% Frames is what the video modulation code (written by Tao SHI) uses.
%
% (from the ETDB documentation:)
% 12 standard video sequences:
% Bus (150 frames)
% City (300 frames)
% Crew (300 frames)
% Flower Garden (250 frames)
% Foreman (300 frames)
% Hall monitor (300 frames)
% Harbour (300 frames)
% Mobile Calendar (300 frames)
% Mother and Daughter (300 frames)
% Soccer (300 frames)
% Stefan (90 frames)
% Tempete (260 frames)
%
% The sequences are stored in YUV 4:2:0 format at CIF (352×288) resolution, and
% 30 frames per second (fps). The video sequences (in uncompressed YUV 4:2:0 format)
% are stored in the "RAW" folder.
video_sequence=struct('name',... 
    {'BUS_352x288_30',...
    'CITY_352x288_30',...
    'CREW_352x288_30',...
    'flower_cif',...
    'FOREMAN_352x288_30',...
    'HallMonitor_cif',...
    'HARBOUR_352x288_30',...
    'MOBILE_352x288_30',...
    'mother-daughter_cif',...
    'SOCCER_352x288_30',...
    'stefan_cif',...
    'tempete_cif',...
     },...
    'n_frames',...
    {150,... % bus
    300,... % city
    300,... % crew
    250,... % flower
    300,... % foreman
    300,... % hall monitor
    300,... % harbour
    300,... % mobile calendar
    300,... % mother-daughter
    300,... % soccer
    90,...  % stefan
    260,... % tempete
    });

for k=1:length(video_sequence)
    single_yuv2png(video_sequence(k).name, video_sequence(k).n_frames);
end
end

function single_yuv2png(video_sequence, n_frames)
video_path=fullfile('dataset', 'video');

video_name=fullfile(video_path,'SFU_etdb','RAW',[video_sequence '.yuv']);
output_folder=['etdb_' video_sequence '_' int2str(n_frames)];
output_path=fullfile(video_path,output_folder);

W = 352;
H = 288;
if exist(output_path,'dir')
    return; % the frames (.png's) for this video sequence already exist
else
    mkdir(output_path)
end
for k=1:n_frames
    % Read one frame from the input YUV file and display it
    frame = loadFileYuv(video_name,W,H,k);
    imshow(frame);
    imwrite(frame,fullfile(output_path,sprintf('%3.3d.png',k)));
end
end
