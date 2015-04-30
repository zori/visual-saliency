function GenerateHeatMaps(VideoFile, InputCSV, ViewMode)
%==========================================================================
% Function to display heat maps on each frame of the input video
% Input Parameters:
%   VideoFile : The input uncompressed video file in YUV 4:2:0 format
%   InputCSV  : The input CSV file that contains the gaze locations
%   ViewMode  : Determines one of the following three display modes:
%       ViewMode = 1 : Display the gaze locations of the first viewing (default)
%       ViewMode = 2 : Display the gaze locations of the second viewing
%
% Example:
%  GenerateHeatMaps('c:\test\foreman.yuv','c:\test\foreman-Screen.csv', 1)
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
        ViewMode = 1;
    end
    
    warning off
    load gauss
    load colormap
    
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
    % Get the size of the gaussian image
    [N M] = size(gauss);
    NoSubjects = 15;
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
                rgb = double(rgb);
                % Generate the heatmap of the current frame using subjects' gaze locations
                output = zeros(H,W);
                for i=1:4:4*NoSubjects
                    Z = GazeLocations(k,i:i+3); % read the gaze location for both viewings, i.e.,  Z=[X1 Y1 X2 Y2]
                    F = FlagMatrix(k,i:i+3);
                    if(ViewMode == 1) % show the heatmap for only the first viewing
                        if(F(1)==1) % if the gaze data is valid
                            X = Z(1);
                            Y = Z(2);
                            if(((X-N/2)>=1)&&((X+N/2)<=W)&&((Y-N/2)>=1)&&((Y+N/2)<=H))
                                output(Y-N/2:Y+N/2-1, X-N/2:X+N/2-1) = output(Y-N/2:Y+N/2-1, X-N/2:X+N/2-1) + gauss;
                            end
                        end
                    end
                    if(ViewMode == 2) % show the heatmap for only the second viewing
                        if(F(3)==1) % if the gaze data is valid
                            X = Z(3);
                            Y = Z(4);
                            if(((X-N/2)>=1)&&((X+N/2)<=W)&&((Y-N/2)>=1)&&((Y+N/2)<=H))
                                output(Y-N/2:Y+N/2-1, X-N/2:X+N/2-1) = output(Y-N/2:Y+N/2-1, X-N/2:X+N/2-1) + gauss;
                            end
                        end
                    end
                end
                % Colorize the generated intensity map using the color map
                normalized_output = round(mat2gray(output)*255);
                output_rgb = ind2rgb(normalized_output,color_map);
                % Overlay/blend the heatmap on the original video frame
                mx1 = max(max(output_rgb(:,:,1)));
                mx2 = max(max(output_rgb(:,:,2)));
                mx3 = max(max(output_rgb(:,:,3)));
                f(:,:,1) = (1-output_rgb(:,:,1)./mx1).*rgb(:,:,1)+(output_rgb(:,:,1)./mx1).*output_rgb(:,:,1)*255;
                f(:,:,2) = (1-output_rgb(:,:,2)./mx2).*rgb(:,:,2)+(output_rgb(:,:,2)./mx2).*output_rgb(:,:,2)*255;
                f(:,:,3) = (1-output_rgb(:,:,3)./mx3).*rgb(:,:,3)+(output_rgb(:,:,3)./mx3).*output_rgb(:,:,3)*255;
                % Show the final result
                imshow(mat2gray(f))
                caption = sprintf('Frame #%d out of %d',k, NumberOfFrames);
                title(caption)
                drawnow
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