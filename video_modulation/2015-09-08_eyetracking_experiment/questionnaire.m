% Zornitsa Kostadinova
% Sep 2015
% 8.5.0.197613 (R2015a)

% NOTE: the order of videos in the eyetracking experiment was decided by
% generating a random permutation:
% 19, 32, 12, 24, 7, 27, 31, 21, 34, 9, 3, 14, 29, 23, 4, 15, 25, 18, 22, 11, 16, 28, 20, 1, 10, 30, 6, 33, 8, 5, 26, 17, 35, 13, 2

% x = ones(1,35);
% x([4, 7, 16]) = 0;
% TIP: disp() puts too many spaces by default; instead, use 
% fprintf(1, '%2d ', x); fprintf('\n');

% 12 participants, 35 videos; was the video normal (0 for unnatural)?
is_normal = [...
    % 01. Trinh, 27 m (wears glasses; did the experiment without them)
    1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  0  1  0  1  1  1  1  0  0  1  1  1  1  1  0  0  1  1
    % 02. MD, 25 m (no glasses)
    1  1  1  0  1  0  0  1  1  1  1  0  0  0  0  1  0  0  1  0  1  0  1  1  0  0  1  0  1  1  0  1  1  0  0
    % 03. Fumiki, 25 m (w/ glasses)
    1  0  0  0  0  0  1  1  1  0  1  1  1  0  0  1  0  0  1  1  1  1  1  1  0  1  1  1  1  1  0  1  0  0  1
    % 04. Segawa, 22 m (w/ glasses)
    1  1  1  0  1  1  1  1  1  1  0  1  1  0  1  1  0  1  1  1  1  1  1  1  0  0  1  1  1  1  1  1  1  1  1
    % 05. Hayashi, 23 m (w/ glasses)
    1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  0  0  1  1  1  1  1  1  0  1  1  1  1  1  1  1  0  1  0
    % 06. Tanji Kaori, 21 f (w/ glasses)
    1  1  0  0  0  1  0  0  1  1  0  0  1  0  0  1  0  0  1  1  1  0  1  1  0  1  0  1  1  1  1  1  0  0  0 
    % 07. Itoh Hayato, 29 m (w/ glasses)
    1  1  1  0  0  0  1  0  1  1  0  1  1  0  0  1  0  0  1  1  0  0  1  1  0  1  0  1  1  0  0  1  0  0  0
    % 08. Yoshiyuki Tamura, 23 m (no glasses)
    1  1  1  0  0  1  1  1  1  1  1  1  1  0  0  1  1  1  1  1  1  1  1  1  0  1  1  1  1  1  1  1  1  0  1
    % 09. Kyota, 24 m (w/ glasses)
    1  1  1  0  1  1  1  0  1  0  1  0  1  0  0  1  1  0  1  0  0  0  1  1  0  1  1  1  1  1  0  0  1  0  1 
    % 10. Kusu, 22 m (w/ glasses)
    1  1  1  0  0  0  1  0  1  1  0  1  1  0  0  1  0  0  1  0  1  0  1  1  0  1  0  1  1  1  0  0  1  0  1
    % 11. Yoshida Toru, 22 m (w/ glasses)
    1  1  1  0  0  0  1  0  0  1  0  0  1  0  0  1  0  0  1  1  1  0  1  0  0  1  0  1  1  1  0  1  0  0  0
    % 12. Ryo Sasaki, 22 m (no glasses)
    1  1  1  1  0  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  0  1  1  1  1  1  1  1  1  0  1
    ];

s = sum(is_normal);
[a, ind] = sort(s);
%   a:  0  3  3  3  3  4  4  5  6  6  6  7  7  7  7  8  8  8  9  9 10 10 10 10 11 11 11 11 11 11 12 12 12 12 12
% ind: 25  4 14 18 34 15 17  5 22 31 33  6  8 11 35 12 20 27 26 32  3  7 10 21  2  9 13 24 28 30  1 16 19 23 29
