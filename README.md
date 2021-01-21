# BS Equalizer (IIR Graphic Equalizer)

## Author | Purpose
- Boris Ermakov-Spektor
- Final Project
- EEL4511: Real-time DSP Applications
- University of Florida

## Hardware Used

- LAUNCHXL-F28379D
- UART to USB Cable
- TLV320AIC23 Audio Codec

## Features

1. Constant 1-Octave wide 10-band IIR filter based 48KHz graphic
equalizer with [-20, 20] dB of range on each band. Equalizes both left
and right channel.
2. Differential Surround effect (Hass Effect) that can run at the same
time as the equalizer.
3. Python GUI application that communicates with DSP over UART to
change equalizer and differential surround parameters.
4. Dual CPUs used, each CPU’s CLA handles the IIR processing for a
single audio channel. The first CPU is additionally used to handle UART
communications.

## Details

- LabFinal_CPU1 contains the Code Composer Studio project for CPU1.
- LabFinal_CPU2 contains the Code Composer Studio project for CPU2.
- LabFinalGUI contains the Python GUI used to communicate with the board.
- Matlab_Filter_Design contains the MATLAB scripts used to generate the IIR filters.

## Note

For this to run in realtime, the Code Composer Projects must be compiled with `-O3` and `--fp_mode=relaxed`

## Libraries

The Code Composer Projects require the following TI library includes:

- C2000Ware_3_02_00_00/libraries/dsp/FPU/c28/include/fpu32
- C2000Ware_3_02_00_00/device_support/f2837xd/headers/include
- C2000Ware_3_02_00_00/device_support/f2837xd/common/include
- C2000Ware_3_02_00_00/driverlib/f2837xd/driverlib