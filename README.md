# bytebass

we make sure your fonky basslines are in the correct scale

## Fast Fourier Transform

To compile: gcc fft.c -o fft -pedantic -Wall -Werror -lm

The output is an array of complex numbers but we can just look at the real part of the complex numbers.

-   Each index of the array corresponds to a frequency. E.g. A[2] = 2Hz
-   The y value at this index is a measure of "how much" of this frequency is present in the original array, at least that's how I understand it
-   Currently in fft.c I just generate a sin wave as input and feed it into the FFT function

To do:

-   Read .wav files and obtain the 1D array needed for FFT function
-   Intepret 1D array result to obtain 1 fundamental frequency
