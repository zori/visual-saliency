% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
classdef MinimisationOption
    enumeration
        TYPE_ORIG_BOOSTING % original code, as Tao SHI implemented it
        TYPE_LLS % non-weighted linear least squares
        TYPE_WLLS % weighted LLS
    end
    enumeration
        AREA_ROI
        AREA_ENTIRE_IMAGE
    end
end
