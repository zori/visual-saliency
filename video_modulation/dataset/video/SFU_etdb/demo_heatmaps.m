%==========================================================================
% Demo Program to Show Heat Maps
% Written by Hadi Hadizadeh (hha54@sfu.ca), October 2010
% Multimedia Communications Laboratoary,
% School of Engineering Science, Simon Fraser University,
% 8888 University Drive, Burnaby, BC, V5A 1S6, Canada
%==========================================================================

% clc
% clear all
% close all

% Input YUV file
VideoFile = fullfile('RAW','FOREMAN_352x288_30_orig.yuv');
% Input CSV file that contains gaze locations
InputCSV = fullfile('CSV','foreman-Screen.csv');
ViewMode = 1; % show the heatmap for the first viewing, use 2 for the second viewing

GenerateHeatMaps(VideoFile, InputCSV, ViewMode);
% Type "help GenerateHeatMaps" in the Matlab's command line to see the
% description of each input parameter of GenerateHeatMaps
