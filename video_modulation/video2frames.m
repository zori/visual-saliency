readerObj = VideoReader('/Users/taoshi/Desktop/mobile_cif.avi');
nFrames = readerObj.NumberOfFrames;

mkdir(datestr(clock,'HHMMSS'));
cd(datestr(clock,'HHMMSS'));

for k = 1:300
    fprintf('%03d\n', k);
    imwrite(read(readerObj, k), strcat(num2str(k, '%03d'), '.png'), 'png');
end
