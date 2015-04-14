i = 90;
%i = 30;

readerObj = VideoReader('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/city/1A.mov');
%readerObj = VideoReader('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/mivue/2A.avi');
image = im2double(read(readerObj, i));

BB = round(importdata('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/city/1B.mat'))';
%BB = round(importdata('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/mivue/2B.mat'))';
crtBB = round(BB(i,:));
% crtBB(1:2) = max([0 0], crtBB(1:2));
% crtBB(3:4) = min(res, crtBB(3:4));
image(crtBB(2):crtBB(4), [crtBB(1):crtBB(1)+2 crtBB(3)-2:crtBB(3)], :) = 1;
image([crtBB(2)-2:crtBB(2) crtBB(4)-2:crtBB(4)], crtBB(1):crtBB(3), :) = 1;

BB = round(importdata('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/city/1C.mat'))';
%BB = round(importdata('/Users/taoshi/Documents/Archives/NII Internship/Experiment video/mivue/2C.mat'))';
crtBB = round(BB(i,:));
% crtBB(1:2) = max([0 0], crtBB(1:2));
% crtBB(3:4) = min(res, crtBB(3:4));
image(crtBB(2):crtBB(4), [crtBB(1):crtBB(1)+3 crtBB(3)-3:crtBB(3)], :) = 0;
image([crtBB(2)-3:crtBB(2) crtBB(4)-3:crtBB(4)], crtBB(1):crtBB(3), :) = 0;

imshow(image);