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

l = v / norm(v, 1);

% 'x', 'exp(x)', 'x^2', 'x^{0.5}', 'log(x+1)', '1 - 1/(x+1)', 'e - exp(1/(x+1))', 'exp(kx)', 'log(k(x+1))'
ex.f = exp(v);                                ex.l = 'exp(x)';
sq.f = v .^ 2;                                sq.l = 'x^2';
sqr.f = v .^ 0.5;                             sqr.l = 'x^{0.5}';
lg.f = log(v + 1);                            lg.l = 'log(x+1)';
recip_sub.f = 1 - 1 ./ (v + 1);               recip_sub.l = '1 - 1/(x+1)';
exr.f = exp(1) - exp(1 ./ (v + 1));           exr.l = 'e - exp(1/(x+1))';

k = 5; % TODO(zori) substitute in the string
ex_k.f = exp(k * v);                          ex_k.l = 'exp(kx)';
ex_1k.f = exp(v ./ k);                        ex_1k.l = 'exp(1/k x)';
lg_k.f = log(k * (v + 1));                    lg_k.l = 'log(k(x+1))';
lg_1k.f = log((v + 1) ./ k);                  lg_1k.l = 'log(1/k (x+1))';

% NOTE(zori) presented on meeting#18
weighting_funcs_initial = [ex, sq, sqr, lg, recip_sub, exr];

weighting_funcs_ex = [ex, ex_k, ex_1k];
weighting_funcs_lg = [lg, lg_k, lg_1k];
weighting_funcs_all = [ex, sq, sqr, lg, recip_sub, exr, ex_k, lg_k
    ];

weighting_funcs = weighting_funcs_all;

% weighting_funcs = weighting_funcs_ex;
weighting_funcs = weighting_funcs_lg;

% normalise
for wf = 1:length(weighting_funcs)
    func = weighting_funcs(wf).f;
    weighting_funcs(wf).f = func / norm(func, 1);
end

set(0, 'DefaultLineLineWidth', 1.5);
plot(v, l)
hold on;

for wf = 1:length(weighting_funcs)
    plot(v, weighting_funcs(wf).f);
end

leg = ['x' {weighting_funcs.l}];
legend(leg, 'Location', 'northwest') % 'southeast')
saveas(fig_handle, 'weighting_functions', 'png'); % or gcf instead of
end
