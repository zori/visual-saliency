% Zornitsa Kostadinova
% May 2015
% 8.5.0.197613 (R2015a)
% based on run_TLD

video_dataset_data();

% three modulation options:
% 1) no modulation (would result in flicker)
% 2) video modulation with simple moving average (as Tao SHI originally implemented)
% 3) exponential smoothing with smoothing weight alpha
modulation = struct('m', {1, 2, 3, 3}, 's', {NaN, NaN, 3, 7});
% choose a proper alpha value
% modulation = struct('m', {3, 3, 3, 3, 3, 3, 3, 3, 3},...
%     's', {6, 8, 9, 10, 11, 12, 13, 14, 15});

% experiment 20_4 (alpha values); increasing 's' monotonically decreases flicker
% saliency, but also quickly fails to attract user gaze
% modulation = struct('m', {3, 3, 3, 3, 3}, 's', {20, 40, 60, 80, 100});

% file with initial bounding box (ROI)
% initial_regions = {'init1.txt'};
% initial_regions = {'init2.txt'};
initial_regions = {'init1.txt', 'init2.txt', 'init3.txt'};

for n = 1:length(sequence_names)
    for ir = 1:length(initial_regions)
        init_workspace;
        opt.sequence_name = [sequence_names{n} suffix];
        for k=1:length(modulation)
                opt.source = make_source_struct(initial_regions{ir}, opt.sequence_name);
                opt.modulation = modulation(k).m;
            opt.smoothing_param_recipr = modulation(k).s;
            opt = run_all_set_opt(opt);
            [bb, conf, weights] = tldSaliency(opt);
        end
    end
end
