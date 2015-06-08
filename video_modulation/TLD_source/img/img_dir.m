% University of Surrey
%
% This file is part of TLD.
%


function images = img_dir(path)
% Looks into 'path' directory and returns a list of images in it

% considered extensions
% ext = {'*.jpeg','*.jpg','*.png','*.pgm'};
% UPDATE(zori) only consider .png, as I keep the image of first frame with the
% bounding box in a .jpg
ext = {'*.png'};

images = [];
for i = 1:length(ext)
    images = [images dir([path ext{i}])];
end

% images are returned with absolute path
for i = 1:length(images)
    images(i).name = [path images(i).name];
end
