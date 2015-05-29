% Copyright 2011 Zdenek Kalal
%
% This file is part of TLD.
%
% TLD is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% TLD is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with TLD.  If not, see <http://www.gnu.org/licenses/>.

% Compiles mex files
clc; clear all; cd TLD_source/mex;

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
    
    % edited by Zori
    % /usr/include/opencv/cv.h
    include = ' -I/opt/local/include/opencv/ -I/opt/local/include/'; 
    libpath = '/opt/local/lib/'; 
    
    files = dir([libpath 'libopencv*.dylib']);
    
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
else
    disp('Unix');
    
    % % /usr/include/opencv/
    % /usr/lib/x86_64-linux-gnu/libstdc++.so.6 % for version `GLIBCXX_3.4.20' (required by /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4)
    % 1. on the DELL (Ubuntu 15.04)
    % include = ' -I/usr/include/opencv/ -I/usr/include/ '; % -L/usr/lib/x86_64-linux-gnu/libstdc++.so.6 '; %-I/home/zori/sources/OpenCV-2.2.0/include/opencv/'; % -I/usr/include/opencv2/highgui';
    % libpath = '/usr/lib/x86_64-linux-gnu/';
    % 2. on the Linux Mint (equivalent to Ubuntu 12.04.5 LTS) desktop (Firenze)
    include = ' -I/usr/local/include/opencv/ -I/usr/local/include/';
    % % /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.9
    libpath = '/usr/local/lib/';
    
    files = dir([libpath 'libopencv*.so']);
    % files = dir([libpath 'libopencv*.so.2.4']);
    % files = dir([libpath 'libopencv*.so.2.2']);
    
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
