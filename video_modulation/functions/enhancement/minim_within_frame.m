% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function frame_out = minim_within_frame(frame_in, frame_other, minim_area,...
    minim_type, lastPyrasAft, cur_modu_saliency, modu_saliency_after_boosting)
switch minim_type
    case MinimisationOption.T_ORIG
        frame_out = frame_in;
    case {MinimisationOption.T_LLS, MinimisationOption.T_WLLS,... 
            MinimisationOption.T_DLLS, MinimisationOption.T_RLLS,...
            MinimisationOption.T_SLLS}
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
        
        switch minim_type
            case MinimisationOption.T_LLS
                p = A \ b;
            case {MinimisationOption.T_WLLS, MinimisationOption.T_DLLS,...
                    MinimisationOption.T_RLLS, MinimisationOption.T_SLLS}
                switch minim_type
                    case MinimisationOption.T_DLLS
                        % take as weight the absolute difference of modulation
                        % saliencies
                        weight = abs(cur_modu_saliency - modu_saliency_after_boosting);
                    case MinimisationOption.T_RLLS
                        % TODO(zori) is that suitable?
                        % weight = 1 ./ (1 + cur_modu_saliency);
                        weight = 1 ./ (1 + modu_saliency_after_boosting);
                    case MinimisationOption.T_SLLS
                        % weight = 1 - cur_modu_saliency;
                        weight = 1 - modu_saliency_after_boosting;
                    case MinimisationOption.T_WLLS
                        warning(['This code does not use the correct weighting schema '...
                            'for minimisation (the within-frame minimisation should '...
                            'employ modulation saliency for the minimisation, and here'...
                            'the flicker saliency is used instead. Left for '...
                            'reproducibility.']);
                        % get the weights: normalised flicker saliency
                        pyras_boosted = make_pyras(frame_in, lastPyrasAft);
                        [~, saliency_flicker] = pyras2saliency(pyras_boosted);
                        weight = saliency_flicker;
                end
                minim_area_weight = weight(minim_area);
                assert(all(minim_area_weight) >= 0)
                minim_area_weight = minim_area_weight / norm(minim_area_weight, 1);
                FW = repmat(minim_area_weight, n_channels, 1);
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
