% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function frame_out = minim_temporal(frame_in, frame_other, minim_area, minim_type, lastPyrasAft)
switch minim_type
    case MinimisationOption.TEMP_O
        frame_out = frame_in;
    case {MinimisationOption.TEMP_L, MinimisationOption.TEMP_W}
        [A, b] = make_LLS_design_matrix(frame_in, frame_other, minim_area);
        switch minim_type
            case MinimisationOption.TEMP_L
                p = A \ b;
            otherwise
                warning(['Implement other types of TEMP_W to differentiate '...
                    'between the weighting mapping (design of the function).']);
                assert(minim_type == MinimisationOption.TEMP_W)
                % NOTE: this uses the original flicker saliency, not the boosted one
                % get the weights: normalised flicker saliency
                pyras_boosted = make_pyras(frame_in, lastPyrasAft);
                [~, saliency_flicker] = pyras2saliency(pyras_boosted);
                
                % linear
                weight = saliency_flicker;
                %             % use the exponentiation
                %             weight = exp(weight);
                weight = weight.^2;
                % weight = weight.^0.5;
                % weight = log(weight + 1);
                
                p = weighted_LLS_minimise(weight, minim_area, A, b);
        end
        frame_out = compute_LLS_result(frame_in, minim_area, A, p);
    otherwise, exit(['Unknown temporal minimisation option '...
            '(defining the between-frames minimisation)']);
end
end
