% Boris Ermakov-Spektor

function write_filter_coeff(fileName, filters)
    % write_filter_coeff - writes coeffecients of IIR filter to file as C arrays
    %

    fileID = fopen(fileName, 'w');

    buff_len = 0;

    for i = 1:length(filters)
        f_size = size(filters(i).Coefficients);
        buff_len = buff_len + f_size(1) * 4;
    end

    f1_size = size(filters(1).Coefficients);

    fprintf(fileID, '#define IIR_FILTER_NUM_STAGES %d\n', f1_size(1));
    fprintf(fileID, 'typedef struct IIR_FILTER {\n');
    fprintf(fileID, '    float taps[IIR_FILTER_NUM_STAGES][6];\n');
    fprintf(fileID, '    float buffer[IIR_FILTER_NUM_STAGES][4];\n');
    fprintf(fileID, '} IIR_FILTER;\n\n');

    for i = 1:length(filters)
        coeff = filters(i).Coefficients;
        f_size = size(coeff);
        fprintf(fileID, 'IIR_FILTER h%d = {', i);
        fprintf(fileID, '.taps = {');

        for j = 1:f_size(1)

            if j == 1
                fprintf(fileID, '{%f, %f, %f, %f, %f, %f}', coeff(j, :));
            else
                fprintf(fileID, '                               {%f, %f, %f, %f, %f, %f}', coeff(j, :));
            end

            if j ~= f_size(1)
                fprintf(fileID, ',\n');
            end

        end

        fprintf(fileID, '}');
        fprintf(fileID, '};');
        fprintf(fileID, '\n\n');
    end

end
