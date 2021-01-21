% Boris Ermakov-Spektor

function filters = gen_equalizer_filters(Fs)
    % gen_equalizer_filters -- Generate FIR filters for equalizer.

    % http://www.sengpielaudio.com/calculator-cutoffFrequencies.htm

    filter_order = 10;

    % 1 octave
    Q = 1.414; % A Q factor of 1.414 will give a bandwidth of 1 octave
    center_freq = [31, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000];

    %Q = 2.145; % 2/3 octave
    %center_freq = [20, 31.5, 50, 80, 125, 200, 315, 500, 800, 1250, 2000, 3150, 5000, 8000, 12500, 20000];

    %Q = 4.318; % 1/3 octave
    %center_freq = [16, 20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000];

    for c = 1:length(center_freq)
        upper_cutoff = center_freq(c) * (sqrt(1 + 1 / (4 * (Q^2))) + (1 / (2 * Q)));
        lower_cutoff = center_freq(c) * (sqrt(1 + 1 / (4 * (Q^2))) - (1 / (2 * Q)));

        % clamp if needed
        if upper_cutoff > Fs / 2
            upper_cutoff = (Fs - 1) / 2;
        end

        if c == 1
            filters(c) = designfilt('lowpassiir', ...
                'HalfPowerFrequency', upper_cutoff, ...
                'FilterOrder', filter_order, ...
                'DesignMethod', 'butter', ...
                'SampleRate', Fs);
        else
            filters(c) = designfilt('bandpassiir', ...
                'HalfPowerFrequency1', lower_cutoff, ...
                'HalfPowerFrequency2', upper_cutoff, ...
                'FilterOrder', filter_order, ...
                'DesignMethod', 'butter', ...
                'SampleRate', Fs);
        end

    end

end
