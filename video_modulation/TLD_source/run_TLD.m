% University of Surrey
%
% This file is part of TLD.
%

init_workspace;

% opt.sequence_name   = 'etdb_foreman_352x288_30';
% opt.sequence_name   = 'orig_bridge_close_300';
% opt.sequence_name   = 'etdb_HallMonitor_cif_300';
% opt.sequence_name   = 'etdb_HARBOUR_352x288_30_300';
% opt.sequence_name   = 'etdb_stefan_cif_90';
% opt.sequence_name   = 'etdb_tempete_cif_260';

% EDIT(zori) I will only work with these two sequences, as ROIs can be chosen so
% that the tracker doesn't lose the object until the end of the video.
opt.sequence_name   = 'etdb_MOBILE_352x288_30_300';
opt.sequence_name   = 'orig_city_300';

opt.modulation = 1; % 1 - no smoothing, 2 - simple averaging within a 6-frame window, or 3 - exponential smoothing
opt.smoothing_param_recipr = 3; % 3, 7

% opt.source          = struct('init_bb_name','init3.txt',... % init3 is just a
% copy of init2; used to indicate in the output files of the experiment that
% only the upper-right corner of the bounding box is considered for modulation (the single-pixel experiment) 
opt.source          = struct('init_bb_name','init2.txt',... % bounding box file
    'camera',0,...
    'input',fullfile('dataset','video',opt.sequence_name,filesep),...
    'bb0',[]); % camera/directory swith, directory_name, initial_bounding_box (if empty, it will be selected by the user)
opt.output          = 'TLD_source/_output/'; mkdir(opt.output); % output directory that will contain bounding boxes + confidence

% min_win             = 1;
min_win             = 10; % minimal size of the object's bounding box in the scanning grid, it may significantly influence speed of TLD, set it to minimal size of the object
patchsize           = [30 30]; % size of normalized patch in the object detector, larger sizes increase discriminability, must be square
fliplr              = 0; % if set to one, the model automatically learns mirrored versions of the object
maxbbox             = 1; % fraction of evaluated bounding boxes in every frame, maxbox = 0 means detector is turned off, if you don't care about speed, set it to 1
update_detector     = 1; % online learning on/off, of 0 detector is trained only in the first frame and then remains fixed
opt.plot            = struct('pex',1,'nex',1,'dt',1,'confidence',1,'target',1,'replace',0,'drawoutput',3,'draw',0,'pts',1,'help', 0,'patch_rescale',1,'save',0); 

% Do-not-change -----------------------------------------------------------

opt.model           = struct('min_win',min_win,'patchsize',patchsize,'fliplr',fliplr,'ncc_thesame',0.95,'valid',0.5,'num_trees',10,'num_features',13,'thr_fern',0.5,'thr_nn',0.65,'thr_nn_valid',0.7);
opt.p_par_init      = struct('num_closest',10,'num_warps',20,'noise',5,'angle',20,'shift',0.02,'scale',0.02); % synthesis of positive examples during initialization
opt.p_par_update    = struct('num_closest',10,'num_warps',10,'noise',5,'angle',10,'shift',0.02,'scale',0.02); % synthesis of positive examples during update
opt.n_par           = struct('overlap',0.2,'num_patches',100); % negative examples initialization/update
opt.tracker         = struct('occlusion',10);
opt.control         = struct('maxbbox',maxbbox,'update_detector',update_detector,'drop_img',1,'repeat',1);

        
% Run TLD -----------------------------------------------------------------
%profile on;
% [bb,conf] = tldExample(opt);
[bb,conf,weights] = tldSaliency(opt); 
%profile off; 
%profile viewer;

% Save results ------------------------------------------------------------
% dlmwrite([opt.output '/tld.txt'],[bb; conf]');
% disp('Results saved to ./_output.');
