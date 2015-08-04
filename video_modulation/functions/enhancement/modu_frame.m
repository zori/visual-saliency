function [ frame_out, W ] = modu_frame( frame_in, frame_ind, diffs, mask, W_orig, lastPyrasAft, minim_type_opt, minim_area_opt )
%MODU_FRAME modulation of given frame, with updated W returned
%   @author Tao

global param;
% make sure choice parameters are defined
assert(exist('minim_type_opt','var') == 1) % good default would be: minim_type_opt = MinimisationOption.TYPE_ORIG_BOOSTING
assert(exist('minim_area_opt','var') == 1)

% pre-enhance
frame_enhanced = enhance(frame_in, diffs, mask, W_orig);
pyrasAft = make_pyras(frame_enhanced, lastPyrasAft);
SAft = process_saliency(get_salimap(pyrasAft));

% mark the location of max saliency after the modulation
% figure; im(SAft); [~, maxind] = max(SAft(:)); [ri,ci] = ind2sub([r, c], maxind); hold on; plot(ci, ri, 'x');
% prepare ehcA
mask_locations = mask ~= 0;
SROI  = SAft(mask_locations);
% ROI_n = length(SROI);
maxROI = max(SROI);
% param.modu_frame_calls = param.modu_frame_calls + 1;
param.max_non_ROI_sal_val(frame_ind) = max(SAft(~mask_locations));
param.max_non_ROI_sal_ind(frame_ind) = find(SAft == 1, 1);
% disp(length(find(SAft == 1))); % how many max salient pixels are there
% compute avg by dividing the following two:
param.ROI_sal(frame_ind) = sum(SROI);
param.nROI_pixels(frame_ind) = length(SROI);
param.ROI_sal_max(frame_ind) = maxROI == 1;
% get a few frames with the locations of the max-salient out-of-ROI pixel
if maxROI ~= 1 % mobile INIT1 MOD1 frame 016 - 038 no boosting; 039 boosting
    % disp('boosting req');
    % db_stop;
    %         [y, x] = ind2sub(size(SAft), param.max_non_ROI_sal_ind(frame_ind));
    %         figure(7); im(frame_in); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_f'], 'jpg');
    %         figure(8); im(SAft); hold on; plot(x,y,'rx','MarkerSize', 20); saveas(gcf, ['/home/work/downloads/' int2str(frame_ind) '_SAft'], 'jpg');
end
fprintf('\t%f', maxROI);
param.ehcA = param.k_a * (1 - maxROI); % param.ehcA becomes 0 if maxROI == 1, resulting in deltaW being a matrix of 0 => no change in W

% get weights
maskPyra = gauss_pyra(mask);
[frame_boosted, deltaW] = boost_HSI(frame_enhanced, diffs, maskPyra);
% TODO(zori) remove the extra boosting, temporarily, to see how statistics
% change
% deltaW = zeros(size(W)); frame_out = frame_enhanced;

% updating of W
W = W_orig + deltaW;
if any(isnan(W(:)))
    error('NaN in weight matrix');
end

switch minim_area_opt
    case MinimisationOption.AREA_ROI
        minim_area = mask_locations;
    case MinimisationOption.AREA_ENTIRE_IMAGE
        minim_area = true(size(SAft));
    otherwise, exit('Unknown minimisation area option');
end

switch minim_type_opt
    case MinimisationOption.TYPE_ORIG_BOOSTING
        frame_out = frame_boosted;
    case {MinimisationOption.TYPE_LLS, MinimisationOption.TYPE_WLLS}
        % (Weighted) LLS minimisation to keeps appearance within ROI as similar to the
        % original as possible.
        
        % (weighted) LLS
        % Want to solve: Ap = b for unknown parameters p = [Theta phi]';
        % A is a n x 2 matrix with the modulated pixels
        % b are the original withing ROI pixel 'observations' (as they are in the
        % frame)
        n_channels = size(frame_in, 3);
        b = []; A = [];  % TODO(zori) use ROI_n * n_channels to pre-allocate
        for c = 1:n_channels
            chan = frame_in(:, :, c);
            b = [b; chan(minim_area)];
            chan = frame_boosted(:, :, c);
            A = [A; chan(minim_area)];
        end
        A = [A ones(length(A), 1)];
        
        if minim_type_opt == MinimisationOption.TYPE_LLS
            p = A \ b;
        else
            assert(minim_type_opt == MinimisationOption.TYPE_WLLS)
            
            % get the weights: normalised flicker saliency
            pyras_boosted = make_pyras(frame_boosted, lastPyrasAft);
            [~, saliency_flicker] = pyras2saliency(pyras_boosted);
            SF_minim_area = saliency_flicker(minim_area);
            % TODO(zori) NORMALISE!!! flicker saliency; make sure the values are
            % positive, as they will be used as weights in the minimisation
            FW = repmat(SF_minim_area, n_channels, 1);
            % % the following creates a huge matrix
            % DFW = diag(FW);
            % p = (A' * DFW * A) \ (A' * DFW * b);
            % % instead, precalculate: define AFW, such that AFW == A' * DFW
            AFW = [A(:,1) .* FW FW]';
            p = (AFW * A) \ (AFW * b);
        end
        % Theta = p(1);
        % phi = p(2);
        lls_out = A * p;
        lls_out = reshape(lls_out, length(lls_out) ./ n_channels, n_channels);
        frame_lls = zeros(size(frame_boosted));
        for c = 1:n_channels
            chan = frame_boosted(:, :, c);
            chan(minim_area) = lls_out(:, c);
            frame_lls(:, :, c) = chan;
        end
        frame_out = frame_lls;
    otherwise, exit('Unknown minimisation type option');
end
end
