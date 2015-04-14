function [ compressedMap, M ] = range_compress( map )
%RANGE_COMPRESS fix the range of any map to [-1, 1] 
%               without change of zero point
%   @author Tao

    absMap = abs(map);
    M = max(absMap(:));
    compressedMap = map / M;

end
