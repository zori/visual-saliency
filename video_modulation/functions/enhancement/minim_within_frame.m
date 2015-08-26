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
        [A, b] = make_LLS_design_matrix(frame_in, frame_other, minim_area);        
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
                p = weighted_LLS_minimise(weight, minim_area, A, b);
        end
        frame_out = compute_LLS_result(frame_in, minim_area, A, p);
    otherwise, exit('Unknown within-frame minimisation option');
end
end
