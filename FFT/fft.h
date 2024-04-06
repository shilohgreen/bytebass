#ifndef MYSTRUCT_H
#define MYSTRUCT_H

typedef struct {
    double value;
    int index;
} Frame;

#endif

double complex* FFT(double complex* x, int N);
double complex* create_alternating_array(double complex* start_pos, int size);
void print_complex(double complex number);
void print_complex_array(double complex* number, int size);
void print_double_array(double *doubles, int size);

double complex* wav_to_arr(char* filepath, SF_INFO sfinfo);
double complex* zero_pad_arr(double complex* arr, int size, long int* padded_size);

int find_max(double complex* arr, int size);
Frame* find_peaks(complex double* arr, int size, int* num_peaks);
void swap(Frame* p1, Frame* p2);
int partition(Frame* arr, int low, int high);
void quickSort(Frame* arr, int low, int high);

double complex* generate_test_array(int size);

