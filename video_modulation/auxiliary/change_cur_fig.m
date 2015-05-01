% Zornitsa Kostadinova
% May 2015
% 8.5.0.197613 (R2015a)
function change_cur_fig(h)
% changes the current figure without changing ("stealing") the focus
set(0,'CurrentFigure',h)
end
