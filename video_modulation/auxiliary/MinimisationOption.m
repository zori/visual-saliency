% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
classdef MinimisationOption
    % area to minimise over
    enumeration
        A_ROI % region of interest
        A_IMG % entire image
    end
    % within-frame type of minimisation
    enumeration
        T_ORIG % original code, as Tao SHI implemented it, no minimisation
        T_LLS % non-weighted linear least squares
        T_WLLS % flicker-saliency weighted LLS; note these are the wrong weights to use for this minimisation, but keep the code for reproducibility
        % modulation-saliency weighted LLS; these should be the correct weighting for this problem
        T_DLLS % use the absolute difference between the modulation saliency before and after enhancing&boosting the frame
        T_RLLS % take the reciprocal of the modulation saliency
        T_SLLS % subtract the modulation saliency from 1
    end
    % temporal (between this and the previous frame) type of minimisation
    enumeration
        TEMP_O % original code, as Tao SHI implemented it, no minimisation
        TEMP_L % LLS
        TEMP_W % weighted LLS % deprecated, use TEMP_WLIN
        TEMP_WLIN     % linear mapping function; same as TEMP_W
        TEMP_WEXP     % exp(x)
        TEMP_WSQ      % x^2
        TEMP_WSQRT    % x^0.5
        TEMP_WLOG     % log(x+1)
        TEMP_WREC     % 1 - 1/(x+1)
        TEMP_WEXPREC  % e - exp(1/(x+1))
    end
end
