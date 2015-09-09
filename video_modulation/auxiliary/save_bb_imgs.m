% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)
function save_bb_imgs()
video_dataset_data();

dataset_folder = fullfile('dataset', 'video');

for n = 1:length(sequence_names)
    sequence_name = fullfile(dataset_folder, [sequence_names{n} suffix]);
    for ir = 1:length(initial_regions)
        init_bb_filename = fullfile(sequence_name, initial_regions{ir});
        bb = dlmread(init_bb_filename);
        first_frame = imread(fullfile(sequence_name, '001.png'));
        ff = figure('visible', 'off');
        imshow(first_frame, 'Border', 'tight');
        rectangle('Position', [bb(1) bb(2) bb(3)-bb(1) bb(4)-bb(2)], ...
            'LineWidth', 5, 'EdgeColor', 'y');
        print(ff, '-r80', '-djpeg', [init_bb_filename(1:end-4) '.jpg']);
    end
end
end
