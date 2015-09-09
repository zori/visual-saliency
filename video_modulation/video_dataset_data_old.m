% Zornitsa Kostadinova
% May 2015
% 8.5.0.197613 (R2015a)
function video_sequences = video_dataset_data_old()
etdb_video_sequences = struct('name',... 
    {'etdb_BUS_352x288_30',...    % 'etdb_CITY_352x288_30',... % same as orig_city
    'etdb_CREW_352x288_30',...
    'etdb_flower_cif',...
    'etdb_FOREMAN_352x288_30',...
    'etdb_HallMonitor_cif',...
    'etdb_HARBOUR_352x288_30',...
    'etdb_MOBILE_352x288_30',...
    'etdb_mother-daughter_cif',...
    'etdb_SOCCER_352x288_30',...
    'etdb_stefan_cif',...
    'etdb_tempete_cif',...
     },...
    'n_frames',...
    {150,... % bus    % 300,... % city % same as orig_city
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

orig_video_sequences = struct('name',... 
    {'orig_bridge_close',...
     'orig_city',... %     'orig_mivue',... % TODO rename .png files
     },...
    'n_frames',...
    {300,... % bridge_close
     300,... % city %   300,... % mivue % TODO rename .png files
    });
    
video_sequences = [etdb_video_sequences, orig_video_sequences];
end
