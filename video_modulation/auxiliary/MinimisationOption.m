% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
classdef MinimisationOption
    enumeration
        ORIG_BOOSTING % original code, as Tao SHI implemented it
        LLS % non-weighted linear least squares
        WLLS % weighted LLS
    end
end
