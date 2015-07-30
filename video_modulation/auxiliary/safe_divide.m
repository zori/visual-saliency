% Zornitsa Kostadinova
% Jul 2015
% 8.5.0.197613 (R2015a)
function result = safe_divide(dividend, divisor)
avoid_zero_division_trick = divisor + (divisor == 0); % adds 1 in case (all values in) divisor are the same
result = dividend ./ avoid_zero_division_trick;
end
