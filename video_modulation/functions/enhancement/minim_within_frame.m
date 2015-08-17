% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function frame_out = minim_within_frame(frame_in, frame_other, minim_area, minim_type, lastPyrasAft)
switch minim_type
    case MinimisationOption.T_ORIG
        frame_out = frame_in;
    case {MinimisationOption.T_LLS, MinimisationOption.T_WLLS}
        % (Weighted) LLS minimisation to keeps appearance of pixels denoted by
        % the mask in `minim_area' as similar as possible to the original.
        
        % (weighted) LLS
        % Want to solve: Ap = b for unknown parameters p = [Theta phi]';
        % A is a n x 2 matrix with the modulated pixels
        % b are the minim_area pixel 'observations' (as they are in frame_other)
        n_channels = size(frame_other, 3);
        b = []; A = [];  % TODO(zori) use ROI_n * n_channels to pre-allocate
        for c = 1:n_channels
            chan = frame_other(:, :, c);
            b = [b; chan(minim_area)];
            chan = frame_in(:, :, c);
            A = [A; chan(minim_area)];
        end
        A = [A ones(length(A), 1)];
        
        if minim_type == MinimisationOption.T_LLS
            p = A \ b;
        else
            assert(minim_type == MinimisationOption.T_WLLS)
            
            % get the weights: normalised flicker saliency
            pyras_boosted = make_pyras(frame_in, lastPyrasAft);
            [~, saliency_flicker] = pyras2saliency(pyras_boosted);
            SF_minim_area = saliency_flicker(minim_area);
            % TODO(zori) The flicker saliency values should be positive (not 
            % just non-negative), as they are used as weights in the
            % minimisation. 
            % But in most cases saliency flicker contains 0. Is it always true?
            % What should be done in those cases?
            % if any(SF_minim_area == 0), disp('saliency flicker contains 0'); end
            assert(all(SF_minim_area) >= 0)
            SF_minim_area = SF_minim_area / norm(SF_minim_area, 1);
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
        minim_out = A * p;
        minim_out = reshape(minim_out, length(minim_out) ./ n_channels, n_channels);
        frame_minim = zeros(size(frame_in));
        for c = 1:n_channels
            chan = frame_in(:, :, c);
            chan(minim_area) = minim_out(:, c);
            frame_minim(:, :, c) = chan;
        end
        frame_out = frame_minim;
    otherwise, exit('Unknown within-frame minimisation option');
end
end
