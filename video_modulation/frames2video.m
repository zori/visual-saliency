function frames2video()
video_sequence = video_dataset_data_old();
for k=1:length(video_sequence)
    video_folder = fullfile('dataset', 'video', [video_sequence(k).name '_' int2str(video_sequence(k).n_frames)]);
    single_frames2video(video_folder, video_sequence(k).n_frames);
end

end
