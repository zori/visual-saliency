%==========================================================================
% Demo Program to Show Gaze Locations
% Written by Hadi Hadizadeh (hha54@sfu.ca), October 2010
% Multimedia Communications Laboratoary,
% School of Engineering Science, Simon Fraser University,
% 8888 University Drive, Burnaby, BC, V5A 1S6, Canada
%==========================================================================

clc
clear all
close all

% Input YUV file
VideoFile = 'RAW\\FOREMAN_352x288_30_orig.yuv';
% Input CSV file that contains gaze locations
InputCSV = 'CSV\\foreman-Screen.csv';
ViewMode = 3; % show the heatmap for both first and second viewing, use 1 for the first viewing, and 2 for the second viewing

ShowGazeLocations(VideoFile, InputCSV, ViewMode);
% Type "help ShowGazeLocations" in the Matlab's command line to see the
% description of each input parameter of ShowGazeLocations
