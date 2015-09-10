% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)

% introduces sequence-related variables to the workspace (name and initial
% regions)

suffix = '_352x288_30_300';

% Most videos in video_dataset_data_old are not suitable. I will only work with
% these sequences, as ROIs can be chosen so that the tracker doesn't lose the
% object until the end of the video.
sequence_names_old = {
    'orig_city' % init1 (tower_bell_orig; smaller); init2 (init_1C - main building part; larger)
    'etdb_MOBILE' % init1 (pigs; larger); init2 (yellow goat head; smaller); init3 (long narrow area under the "head" of the tower)
    }';

sequence_names_new = {
    'beach'
    'newport'
	'palace'
    'vegas'
    }';

% those were not used in the eye-tracking experiment, but for the dummy video
% shown before the actual eye-tracking recording begins
% both sequences only have defined initial regions 'init1.txt', 'init2.txt'
sequence_names_extra = {
    'baseball'
    'canal'
    }';

sequence_names = [sequence_names_old sequence_names_new];

initial_regions = {'init1.txt', 'init2.txt', 'init3.txt'};
