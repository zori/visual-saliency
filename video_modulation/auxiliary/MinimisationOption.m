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
        T_SLLS % modulation-saliency weighted LLS; these should be the correct weighting for this problem
    end
    % temporal (between this and the previous frame) type of minimisation
    enumeration
        TEMP_O % original code, as Tao SHI implemented it, no minimisation
        TEMP_L % LLS
        TEMP_W % weighted LLS
    end
end
