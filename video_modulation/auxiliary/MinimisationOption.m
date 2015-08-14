% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
classdef MinimisationOption
    enumeration % in-frame type of minimisation (if any)
        T_ORIG % original code, as Tao SHI implemented it, no minimisation
        T_LLS % non-weighted linear least squares
        T_WLLS % weighted LLS
    end
    enumeration % area to minimise on
        A_ROI
        A_IMG % entire image
    end
    enumeration % temporal (between this and the previous frame) type of minimisation (if any)
        TEMP_O % original code, as Tao SHI implemented it, no minimisation
        TEMP_L % LLS
        TEMP_W % weighted LLS
    end
end
