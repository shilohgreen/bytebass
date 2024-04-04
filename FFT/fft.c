#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "fft.h"

int main(int argc, char *argv[]) {
    int size = (int) pow(2,10);
    printf("size: %d\n", size);
    double complex *input = generate_test_array(size);
    double complex* res = FFT(input, size);
    print_complex_array(res, size);
    free(input);
    free(res);
    return 0;
}

double complex* FFT(double complex* P, int n) {
    int i;
    double complex* P_e, *P_o, *y_e, *y_o;
    double complex* y = malloc(n * sizeof(double complex));;
    complex w = cexp((2*M_PI*I)/n);
    if (n == 1) {
        double complex* P_new = malloc(sizeof(double complex));
        P_new[0] = P[0];
        return P_new;
    }
    P_e = create_alternating_array(&P[0], n/2);
    P_o = create_alternating_array(&P[1], n/2);
    y_e = FFT(P_e, n/2);
    y_o = FFT(P_o, n/2);

    for (i = 0; i < n; i++) {
        y[i] = 0;
    }
    for (i = 0; i < n/2; i++) {
        y[i] = y_e[i] + cpow(w, i)*y_o[i];
        y[i+n/2] = y_e[i] - cpow(w, i)*y_o[i];
    }
    // printf("Addresses: %p %p %p %p\n", (void *)P_e, (void *)P_o, (void *)y_e, (void *)y_o);
    free(P_e);
    free(P_o);
    free(y_e);
    free(y_o);
    return y;
}

double complex* create_alternating_array(double complex* start, int size) {
    int i = 0;
    double complex* pos = start;
    double complex* arr = (double complex*) malloc(size * sizeof(double complex));
    while (i < size) {
        arr[i] = pos[0];
        i++;
        pos+=2;
    }
    return arr;
}

void print_complex(double complex number) {
    printf("%.10f + %.10fi\n", creal(number), cimag(number));
}

void print_complex_array(double complex* number, int size) {
    int i = 0;
    double complex* pos = number;
    while (i < size) {
        print_complex(pos[0]);
        pos++;
        i++;
    }
}

// Testing functions
double complex* generate_test_array(int size) {
    double complex* arr = malloc(size * sizeof(double complex));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        double complex x = (double complex) i/10;
        arr[i] = csin(x);
    }
    return arr;
}
