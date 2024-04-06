# bytebass

we make sure your fonky basslines are in the correct scale

## Fast Fourier Transform

To compile: gcc fft.c -o fft -pedantic -Wall -Werror -lm

To run: ./fft <insert filename here\>

### How it works

1. Extracts array of doubles from wav file and changes the format to complex double type (Imaginary part is zero, FFT requires an array of complex numbers as input)
2. Complex number array is padded with zeros until the array is of some length 2^k (FFT requires array of length 2^k)
3. FFT returns a complex number array same length as its input
4. How this array is intepreted is left to us, I decided to extract all the peaks (element is a peak if element to left and right is smaller)
5. Return top 5 peaks by decreasing order of magnitude
