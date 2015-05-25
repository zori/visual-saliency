% Zornitsa Kostadinova
% May 2015
% 8.5.0.197613 (R2015a)
% based on run_TLD

sequence_names={
    'etdb_HallMonitor_cif_300',...
    'etdb_HARBOUR_352x288_30_300',...
    'etdb_MOBILE_352x288_30_300',...
    'etdb_stefan_cif_90',...
    'etdb_tempete_cif_260',...
    'orig_bridge_close_300',...
    'orig_city_300',...
    };

sequence_names={
    'orig_city_300',... % init1 (tower_bell_orig); init2 (init_1C - main building part)
    };

sequence_names={
    'etdb_MOBILE_352x288_30_300',...
    };

% three modulation options:
% 1) no modulation (would result in flicker)
% 2) video modulation with simple moving average (as Tao SHI originally implemented)
% 3) exponential smoothing with smoothing weight alpha
modulation=struct('m',{1,2,3,3},'s',{NaN, NaN,3,7});

initial_regions={'init1.txt','init2.txt'}; % bounding box file
initial_regions={'init2.txt'};
initial_regions={'init1.txt'};
for ir=1:length(initial_regions)
    init_workspace;
    % opt=struct;
    for n = 1:length(sequence_names)
        init_workspace;
        opt.sequence_name = sequence_names{n};
        for k=1:4
                % opt.source          = struct('camera',0,'input',fullfile('dataset','video',opt.sequence_name,filesep),'bb0',[]);
                opt.source = struct('init_bb_name',initial_regions{ir},...
                    'camera',0,...
                    'input',fullfile('dataset','video',opt.sequence_name,filesep),...
                    'bb0',[]); % camera/directory swith, directory_name, initial_bounding_box (if empty, it will be selected by the user)
                opt.modulation=modulation(k).m;
            opt.smoothing_param_recipr=modulation(k).s;
            opt=run_all_set_opt(opt);
            [bb,conf,weights] = tldSaliency(opt);
        end
    end
end
