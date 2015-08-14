% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
classdef MinimisationOption
    enumeration % type of minimisation (if any)
        T_ORIG % original code, as Tao SHI implemented it
        T_LLS % non-weighted linear least squares
        T_WLLS % weighted LLS
    end
    enumeration % area to minimise on
        A_ROI
        A_IMG % entire image
    end
end
