% Boris Ermakov-Spektor

function y = band_equalizer(gains, h, x, L)
    % band_equalizer -- Apply equalizer to input signal
    %

    if length(gains) ~= length(h)
        disp('Gain vector must be as long as filter vector');
        return
    end

    y = zeros(1, length(x));

    for c = 1:length(h)
        gain = 10^(gains(c) / 20);
        y = y + sosfilt(h(c).Coefficients, x) * gain;
    end

end
