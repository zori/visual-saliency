% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)

% introduces sequence-related variables to the workspace (name and initial
% regions)
sequence_names = {
    'orig_city',... % init1 (tower_bell_orig; smaller); init2 (init_1C - main building part; larger)
    'etdb_MOBILE',... % init1 (pigs; larger); init2 (yellow goat head; smaller); init3 (long narrow area under the "head" of the tower)
    'beach',...
    'newport',...
	'palace',...
    'vegas'
    };

sequence_names = {
    'beach',...
    'newport',...
	'palace',...
    'vegas'
    };

sequence_names = {
    'orig_city',... % init1 (tower_bell_orig; smaller); init2 (init_1C - main building part; larger)
    'etdb_MOBILE'
    };

initial_regions = {'init1.txt', 'init2.txt', 'init3.txt'};
initial_regions = {'init3.txt'};
