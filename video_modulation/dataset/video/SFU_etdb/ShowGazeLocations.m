function ShowGazeLocations(VideoFile,InputCSV,ViewMode)
%==========================================================================
% Function to display gaze locations on each frame of the input video
% Input Parameters:
%   VideoFile : The input uncompressed video file in YUV 4:2:0 format
%   InputCSV  : The input CSV file that contains the gaze locations
%   ViewMode  : Determines one of the following three display modes:
%       ViewMode = 1 : Display the gaze locations of the first viewing
%       ViewMode = 2 : Display the gaze locations of the second viewing
%       ViewMode = 3 : Display the gaze locations of both viewing (default)
%
% Example:
%  ShowGazeLocations('c:\test\foreman.yuv','c:\test\foreman-Screen.csv', 3)
%
% Written by Hadi Hadizadeh (hha54@sfu.ca), October 2010 - January 2011
% Multimedia Communications Laboratoary,
% School of Engineering Science, Simon Fraser University,
% 8888 University Drive, Burnaby, BC, V5A 1S6, Canada
%==========================================================================
if( (nargin == 0) || (nargin == 1))
    disp 'Not enough input arguments! Please read the help.'
else
    if (nargin == 2)
        ViewMode = 3;
    end
    
    %----------------------------------------------------------------------
    % Compute the flag matrix to remove outliers from the gaza data
    GenerateMask(InputCSV,30); % comment this line if you like to use the pre-computed flag matrix
    % Type 'help GenerateMask' for more info
    % Load the relevant flag matrix from the file
    [path name] = fileparts(InputCSV);
    switch lower(name)
        case {'bus-screen'}
            maskfile = {'bus-mask.csv'};
        case {'city-screen'}
            maskfile = {'city-mask.csv'};
        case {'crew-screen'}
            maskfile = {'crew-mask.csv'};
        case {'flower-screen'}
            maskfile = {'flower-mask.csv'};
        case {'foreman-screen'}
            maskfile = {'foreman-mask.csv'};
        case {'hall-screen'}
            maskfile = {'hall-mask.csv'};
        case {'harbour-screen'}
            maskfile = {'harbour-mask.csv'};
        case {'mobile-screen'}
            maskfile = {'mobile-mask.csv'};
        case {'mother-screen'}
            maskfile = {'mother-mask.csv'};
        case {'soccer-screen'}
            maskfile = {'soccer-mask.csv'};
        case {'stefan-screen'}
            maskfile = {'stefan-mask.csv'};
        case {'tempete-screen'}
            maskfile = {'tempete-mask.csv'};
    end
    [FlagMatrix texts] = xlsread(sprintf('%s\\%s',char(path),char(maskfile)));
    %----------------------------------------------------------------------
    NoSubjects = 15;
    % Define the set of colors for displaying the gaze locations
    Colors = [1 0 0; 0 1 0; 0 0 1; 1 1 0; 1 0 1; 0 1 1; 0.1 0.3 0;
        1 1 1; 0.5 0.5 0.5; 1 0.5 0; 0 0.5 1; 0.7 0.2 0.5;
        0.4 0.3 0.7; 0.3 0.8 0.6; 0.1 0.5 0.3];
    % Define the size of the input video
    W = 352;
    H = 288;
    if(exist(InputCSV,'file'))
        if(exist(VideoFile, 'file'))
            % Read the gaze locations from the input CSV file
            [GazeLocations texts] = xlsread(InputCSV);
            [NumberOfFrames tmp] = size(GazeLocations);
            str = sprintf('Processing %d frames...', NumberOfFrames);
            disp(str)
            disp('Press Ctrl+C to exit...')
            % Loop over frames and display the gaze locations over each videe frame
            for k=1:NumberOfFrames
                % Read one frame from the input YUV file and display it
                rgb = loadFileYuv(VideoFile,W,H,k);
                imshow(rgb)
                hold on
                j=1;
                % Overlay the gaze locations on the current frame
                for i=1:4:4*NoSubjects
                    Z = GazeLocations(k,i:i+3); % read the gaze location for both viewings, i.e.,  Z=[X1 Y1 X2 Y2]
                    F = FlagMatrix(k,i:i+3);
                    if(ViewMode == 1) % display the gaze locations for the first viewing
                        if(F(1)==1) % if the gaze data is valid
                            plot([Z(1) Z(1)],[Z(2) Z(2)],'-o', 'Color', Colors(j,:), 'LineWidth',2,'MarkerSize',10)
                        end
                    end
                    if(ViewMode == 2) % display the gaze locations for the second viewing
                        if(F(3)==1) % if the gaze data is valid
                            plot([Z(3) Z(3)],[Z(4) Z(4)],'-o', 'Color', Colors(j,:), 'LineWidth',2,'MarkerSize',10)
                        end
                    end
                    if(ViewMode == 3) % display the gaze locations for both viewing
                        if((F(1)==1)&&(F(3)==1)) % if the gaze data for both viewings is valid
                            plot([Z(1) Z(3)],[Z(2) Z(4)],'-o', 'Color', Colors(j,:), 'LineWidth',2,'MarkerSize',10)
                        end
                        if((F(1)==1)&&(F(3)==0)) % if the gaze data for only the first viewing is valid
                            plot([Z(1) Z(1)],[Z(2) Z(2)],'-o', 'Color', Colors(j,:), 'LineWidth',2,'MarkerSize',10)
                        end
                        if((F(1)==0)&&(F(3)==1)) % if the gaze data for only the second viewing is valid
                            plot([Z(3) Z(3)],[Z(4) Z(4)],'-o', 'Color', Colors(j,:), 'LineWidth',2,'MarkerSize',10)
                        end
                    end
                    j = j + 1;
                end
                caption = sprintf('Frame #%d out of %d',k, NumberOfFrames);
                title(caption)
                drawnow
                refresh
            end
            disp 'Done!'
        else
            disp 'Error! Input video file not found!'
        end
    else
        disp 'Error! Input CSV file not found!'
    end
end
end