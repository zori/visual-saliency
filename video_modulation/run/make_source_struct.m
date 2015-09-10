% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)
function source = make_source_struct(initial_region, sequence_name)
source = struct('init_bb_name', initial_region,... % bounding box file
    'camera', 0, ... % camera/directory swith
    'input', fullfile('dataset', 'video', sequence_name, filesep),... % directory_name
    'bb0', []); % depricated, 'init_bb_name' is used now % initial_bounding_box (if empty, it will be selected by the user)
end
