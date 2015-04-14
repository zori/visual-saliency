function [ pyraS ] = shift_pyra( pryaO )
%SHIFT_PYRA create S pyramids form O pyramid, given predefined directions 
%   @author Tao

    global param;
    
    pyraS = cell(param.motionDirects, param.pyraScales+1);
    for directIdx = 1:param.motionDirects
        motion = param.motionVectors{directIdx};
        for scaleIdx = 1:(param.pyraScales+1)
            res = param.pyraRes{scaleIdx};
            shiftedImage = circshift(pryaO{directIdx, scaleIdx}, motion);
            if motion(1)
                shiftedImage(mod(motion(1), res(1) + 1), :) = 0;
            end
            if motion(2)
                shiftedImage(:, mod(motion(2), res(2) + 1)) = 0;
            end
            pyraS{directIdx, scaleIdx} = shiftedImage;
        end
    end

end