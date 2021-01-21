% Boris Ermakov-Spektor

function eq_audio(gains, inputFile, outputFile)
% eq_audio -- Read in audio, generate filters, and output equalized audio
%   See gen_equalizer_filters for which frequencies correspond to the gain
%   indexes.

L = 10000; % block size for convolution
[x, Fs] = audioread(inputFile);
sx = size(x);
eq = gen_equalizer_filters(Fs);
if sx(2) == 2
    y_left = band_equalizer(gains, eq, x(:,1)', L);
    y_right = band_equalizer(gains, eq, x(:,2)', L);
    %y_left = y_left/max(abs(y_left));
    %y_right = y_right/max(abs(y_right));
    audiowrite(outputFile, [y_left(:), y_right(:)], Fs);
else
    y = band_equalizer(gains, eq, x(:,1)', 1024);
    %y = y/max(abs(y));
    audiowrite(outputFile, y, Fs);
end

end