function GenerateMask(InputCSV, Filter_Length)
%==========================================================================
% Function to generate a binary mask file for a given CSV file containing
% the gaze data.
% Input Parameters:
%   InputCSV  : The input CSV file that contains the gaze locations
%   Filter_Length : The length of the constant periods to be removed
% The output is stored in an output CSV file based on the name of the input
% CSV file. For example, the output file will be "city-mask.csv" when the
% input is "city-Screen.csv".
%
% This function performs several tasks: 1) it flags as invalid those gaze
% locations that are out of the frame boundary or those gaze locations that
% are very close to (less than 5 pixels) or at the frame boundaries. 2) it
% flags as invalid those periods in which the gaze locations remains
% constant for more than N successive frames (or all frames) where N equals
% to the specified Filter_Length. In the computed binary mask, a zero means
% an ivalid data, and a one means a valid data.
%
% Example:
%  GenerateMask('c:\test\foreman-Screen.csv', 30)
%
% Written by Hadi Hadizadeh (hha54@sfu.ca), January 2011
% Multimedia Communications Laboratoary,
% School of Engineering Science, Simon Fraser University,
% 8888 University Drive, Burnaby, BC, V5A 1S6, Canada
%==========================================================================
if( (nargin == 0) )
    disp 'Not enough input arguments! Please read the help.'
    return
end
if( (nargin == 1) )
    Filter_Length = 30; % 30 frames is the default value
end

disp 'Generating the flag matrix...'

NumberOfSubjects = 15;
%--------------------------------------------------------------------------
%reading the input CSV file containing the gaze data
%--------------------------------------------------------------------------
[GazeLocations texts] = xlsread(InputCSV);
[NumberOfFrames tmp] = size(GazeLocations);

%generating the binary mask file, flag_matrix=1 means valid gaze data
%--------------------------------------------------------------------------
% Task 1: removing those gaze locations that are out of frame or at the frame boundaries
%--------------------------------------------------------------------------
flag_matrix = zeros(size(GazeLocations));
for j=1:NumberOfFrames
    for i=1:2:NumberOfSubjects*4
        flag_matrix(j,i)=1; % the x-component of the gaze location
        flag_matrix(j,i+1)=1; % the y-component of the gaze location
        if((GazeLocations(j,i)<5)||(GazeLocations(j,i)>348)||(GazeLocations(j,i+1)<5)||(GazeLocations(j,i+1)>284))
            flag_matrix(j,i)=0;
            flag_matrix(j,i+1)=0;
        end
    end
end
%--------------------------------------------------------------------------
% Task 2: removing those constant periods of length larger than the specified Filter_Length
%--------------------------------------------------------------------------
for i=1:2:NumberOfSubjects*4 % loop over x-components
    for j=1:NumberOfFrames
        if((j<=Filter_Length*2)) % look forward
            if((j+2*Filter_Length)<=NumberOfFrames)
                if(var(GazeLocations(j:2:j+2*Filter_Length,i))<0.1)
                    flag_matrix(j,i)=0;
                    flag_matrix(j,i+1)=0;
                end
            end
        else %look backward
            if((j-2*Filter_Length)>=1)
                if(var(GazeLocations(j:-2:j-2*Filter_Length,i))<0.1)
                    flag_matrix(j,i)=0;
                    flag_matrix(j,i+1)=0;
                end
            end
        end
    end
    if(var(GazeLocations(:,i))<0.1) %deal with all-constant cases
        flag_matrix(:,i)=0;
        flag_matrix(:,i+1)=0;
    end
end
for i=2:2:NumberOfSubjects*4 % loop over y-components
    for j=1:NumberOfFrames
        if((j<=Filter_Length*2)) % look forward
            if((j+2*Filter_Length)<=NumberOfFrames)
                if(var(GazeLocations(j:2:j+2*Filter_Length,i))<0.1)
                    flag_matrix(j,i)=0;
                    flag_matrix(j,i-1)=0;
                end
            end
        else % look backward
            if((j-2*Filter_Length)>=1)
                if(var(GazeLocations(j:-2:j-2*Filter_Length,i))<0.1)
                    flag_matrix(j,i)=0;
                    flag_matrix(j,i-1)=0;
                end
            end
        end
    end
    if(var(GazeLocations(:,i))<0.1) %deal with all-constant cases
        flag_matrix(:,i)=0;
        flag_matrix(:,i-1)=0;
    end
end
%--------------------------------------------------------------------------
% now save the computed flag matrix into a file
%--------------------------------------------------------------------------
[path name] = fileparts(InputCSV);
switch lower(name)
    case {'bus-screen'}
        outfile = {'bus-mask.csv'};
    case {'city-screen'}
        outfile = {'city-mask.csv'};
    case {'crew-screen'}
        outfile = {'crew-mask.csv'};
    case {'flower-screen'}
        outfile = {'flower-mask.csv'};
    case {'foreman-screen'}
        outfile = {'foreman-mask.csv'};
    case {'hall-screen'}
        outfile = {'hall-mask.csv'};
    case {'harbour-screen'}
        outfile = {'harbour-mask.csv'};
    case {'mobile-screen'}
        outfile = {'mobile-mask.csv'};
    case {'mother-screen'}
        outfile = {'mother-mask.csv'};
    case {'soccer-screen'}
        outfile = {'soccer-mask.csv'};
    case {'stefan-screen'}
        outfile = {'stefan-mask.csv'};
    case {'tempete-screen'}
        outfile = {'tempete-mask.csv'};
end

fid = fopen(sprintf('%s\\%s',char(path),char(outfile)),'w');
fprintf(fid,'Mask file for gaze locations - one means valid gaze data\n');
n=1;
for i=1:4:60
    fprintf(fid,'P%dV1 - X,',n);
    fprintf(fid,'P%dV1 - Y,',n);
    fprintf(fid,'P%dV2 - X,',n);
    fprintf(fid,'P%dV2 - Y,',n);
    n = n + 1;
end
fprintf(fid,'\n');
fclose(fid);
dlmwrite(sprintf('%s\\%s',char(path),char(outfile)),flag_matrix,'-append');
disp 'Done.'
end