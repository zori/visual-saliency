% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function frame_out = minim_temporal(frame_in, frame_other, minim_area, minim_type, lastPyrasAft)
switch minim_type
    case MinimisationOption.TEMP_O
        frame_out = frame_in;
    otherwise
        [A, b] = make_LLS_design_matrix(frame_in, frame_other, minim_area);
        switch minim_type
            case MinimisationOption.TEMP_L
                p = A \ b;
            otherwise
                % NOTE: this uses the original flicker saliency, not the boosted one
                % get the weights: normalised flicker saliency
                pyras_boosted = make_pyras(frame_in, lastPyrasAft);
                [~, saliency_flicker] = pyras2saliency(pyras_boosted);
                weight = saliency_flicker;
                switch minim_type
                    case MinimisationOption.TEMP_W
                        exit('Deprecated option. Use the equivalent TEMP_WLIN.');
                    case MinimisationOption.TEMP_WLIN
                        % linear
                    case MinimisationOption.TEMP_WEXP
                        weight = exp(weight);
                        % multiplicative constant doesn't matter, because of the
                        % normalisation
                        % weight = 0.5 * exp(weight);
                    case MinimisationOption.TEMP_WSQ
                        weight = weight.^2;
                    case MinimisationOption.TEMP_WSQRT
                        weight = weight.^0.5;
                    case MinimisationOption.TEMP_WLOG
                        weight = log(weight + 1);
                    case MinimisationOption.TEMP_WREC
                        weight = 1 - 1 ./ (weight + 1);
                    case MinimisationOption.TEMP_WEXPREC
                        weight = exp(1) - exp(1 ./ (weight + 1));
                    otherwise, exit(['Unknown temporal minimisation option '...
                            '(defining the between-frames minimisation)'])
                end
                p = weighted_LLS_minimise(weight, minim_area, A, b);
        end
        frame_out = compute_LLS_result(frame_in, minim_area, A, p);
end
end
