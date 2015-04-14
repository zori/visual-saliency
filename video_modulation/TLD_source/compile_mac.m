% University of Surrey
%
% This file is part of TLD.
%


% Compiles mex files
clc; clear all; cd mex;

if ispc
disp('PC');
include = ' -Ic:\OpenCV2.2\include\opencv\ -Ic:\OpenCV2.2\include\';
libpath = 'c:\OpenCV2.2\lib\';
files = dir([libpath '*.lib']);

lib = [];
for i = 1:length(files),
lib = [lib ' ' libpath files(i).name];
end

eval(['mex lk.cpp -O' include lib]);
mex -O -c tld.cpp
mex -O fern.cpp tld.obj
mex -O linkagemex.cpp
mex -O bb_overlap.cpp
mex -O warp.cpp
mex -O distance.cpp
elseif ismac
disp('Mac');

include = ' -I/Users/kma/opencv2.4.0/include/opencv  -I/Users/kma/opencv2.4.0/include -I/usr/include/'; 
libpath = '/Users/kma/opencv2.4.0/lib/'; 

files = dir([libpath 'libopencv*.dylib']);

lib = [];
for i = 1:length(files),
lib = [lib ' ' libpath files(i).name];
end
include
lib
eval(['mex lk.cpp -O' include lib]);
mex -O -c tld.cpp
mex -O fern.cpp tld.o
mex -O linkagemex.cpp
mex -O bb_overlap.cpp
mex -O warp.cpp
mex -O distance.cpp
else
disp('Unix');

include = ' -I/usr/local/include/opencv/ -I/usr/local/include/';
libpath = '/usr/local/lib/';

files = dir([libpath 'libopencv*.so.2.2']);

lib = [];
for i = 1:length(files),
lib = [lib ' ' libpath files(i).name];
end

eval(['mex lk.cpp -O' include lib]);
mex -O -c tld.cpp
mex -O fern.cpp tld.o
mex -O linkagemex.cpp
mex -O bb_overlap.cpp
mex -O warp.cpp
mex -O distance.cpp

end


cd ..
disp('Compilation finished.');

