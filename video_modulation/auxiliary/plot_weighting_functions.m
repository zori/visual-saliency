% Zornitsa Kostadinova
% Aug 2015
% 8.5.0.197613 (R2015a)
function plot_weighting_functions()
% see minim_temporal()
v = 0:0.01:1;
fig_handle = 8;
try
    close(fig_handle);
end
figure(fig_handle);

l = v/norm(v,1);
ex = exp(v);                        ex = ex/norm(ex, 1);
sq = v.^2;                          sq = sq/norm(sq, 1);
sqr = v.^0.5;                       sqr = sqr/norm(sqr, 1);
lg = log(v+1);                      lg = lg/norm(lg, 1);
recip_sub = 1 - 1 ./ (v + 1);       recip_sub = recip_sub/norm(recip_sub, 1);
exr = exp(1) - exp(1 ./ (v + 1));   exr = exr/norm(exr, 1);

set(0, 'DefaultLineLineWidth', 1.5);
plot(v, l)
hold on;
plot(v, ex)
plot(v, sq)
plot(v, sqr)
plot(v, lg)
plot(v, recip_sub);
plot(v, exr)

legend('x', 'exp(x)', 'x^2', 'x^{0.5}', 'log(x+1)',...
    '1 - 1/(x+1)', 'e - exp(1/(x+1))', 'Location', 'southeast')
saveas(fig_handle, 'weighting_functions', 'png'); % or gcf instead of
% fig_handle
end
