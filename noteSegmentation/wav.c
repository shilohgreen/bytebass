#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "wav.h"

// // WAV file header struct
// typedef struct
// {
//     char chunk_id[4];
//     uint32_t chunk_size;
//     char format[4];
//     char subchunk1_id[4];
//     uint32_t subchunk1_size;
//     uint16_t audio_format;
//     uint16_t num_channels;
//     uint32_t sample_rate;
//     uint32_t byte_rate;
//     uint16_t block_align;
//     uint16_t bits_per_sample;
//     char subchunk2_id[4];
//     uint32_t subchunk2_size;
// } WavHeader;

// int main()
// {
//     FILE *file;
//     WavHeader header;
//     uint16_t *audio_data;
//     int num_samples;

//     // Open the WAV file
//     file = fopen("C_major.wav", "rb");
//     if (!file)
//     {
//         printf("Error opening file\n");
//         return 1;
//     }

//     // Read the WAV file header
//     fread(&header, sizeof(WavHeader), 1, file);

//     // Allocate memory for audio data
//     num_samples = header.subchunk2_size / (header.bits_per_sample / 8);
//     audio_data = (uint16_t *)malloc(num_samples * sizeof(uint16_t));

//     // Read audio data from the file
//     fread(audio_data, sizeof(uint16_t), num_samples, file);

//     // Close the file
//     fclose(file);

//     // Process the audio data as needed
//     printf("NUM CHANNELS: %d\n", header.num_channels);
//     printf("SAMPLE RATE: %d\n", header.sample_rate);
//     printf("BYTE RATE: %d\n", header.byte_rate);
//     printf("BITS PER SAMPLE: %d\n", header.bits_per_sample);
//     printf("NUM SAMPLES: %d\n", num_samples);

//     int T = 5000; // Window size
//     int num_onsets = detect_onsets(audio_data, T, num_samples);
//     printf("Total number of note onsets detected: %d\n", num_onsets);

//     // Free allocated memory
//     free(audio_data);

//     return 0;
// }

// // // Function to rectify audio_data[i]
// // double rectify(uint16_t sample, int t_n, int t)
// // {
// //     double sum = 0;
// //     for (int tau = t_n; tau <= t; tau++)
// //     {
// //         sum += audio_data[tau] * audio_data[tau]; // Square of the amplitude
// //     }
// //     return sum / (t - t_n); // Calculate average power
// // }

// // Function to detect note onsets
// // int detect_onsets(uint16_t *audio_data, int T, int audio_length)
// // {
// //     int n = 0;
// //     int t_n = 0;
// //     int t;
// //     int onset_idx = 0;
// //     float k;

// //     float *NAE_t = (float *)malloc(sizeof(float) * audio_length);
// //     int *onsets = (int *)malloc(sizeof(int) * audio_length);
// //     NAE_t[0] = 0;

// //     for (t = 1; t <= 2 * T; t++)
// //     {
// //         k = 1.0 / (t - t_n);
// //         NAE_t[t] = k * (NAE_t[t - 1] + abs(audio_data[t]));
// //         printf("NAE(%d) = %f\n", t, NAE_t[t]);
// //     }

// //     for (t = 2 * T + 1; t < audio_length - T; t++)
// //     {
// //         k = 1.0 / (t - t_n);
// //         NAE_t[t] = k * (NAE_t[t - 1] + abs(audio_data[t]));
// //         printf("t: %d, LEFT: %f, MIDDLE: %f, RIGHT: %f\n", t - T, NAE_t[t - (2 * T)], NAE_t[t - T], NAE_t[t]);
// //         if (NAE_t[t - (2 * T)] > NAE_t[t - T] && NAE_t[t - T] < NAE_t[t])
// //         {
// //             printf("Onset detected at %d with NAE value: %f\n", t, NAE_t[t]);
// //             onsets[onset_idx] = t;
// //             n++;
// //             t_n = t;
// //             onset_idx++;
// //         }
// //         printf("NAE(%d) = %f\n", t, NAE_t[t]);
// //     }

// //     return onset_idx; // Return the total number of note onsets detected
// // }

// // Function to calculate the cumulative sum of power envelope
// void calculate_cumulative_sum(float *audio_data, float *cumulative_sum, int num_samples)
// {
//     cumulative_sum[0] = audio_data[0];
//     for (int i = 1; i < num_samples; i++)
//     {
//         cumulative_sum[i] = cumulative_sum[i - 1] + audio_data[i];
//     }
// }

// // Function to calculate NAE using cumulative sum
// float calculate_NAE(float *cumulative_sum, int tn, int t)
// {
//     if (t == tn)
//     {
//         return cumulative_sum[t];
//     }
//     return (cumulative_sum[t] - cumulative_sum[tn]) / (t - tn);
// }

// // Function to detect note onsets
// void detect_note_onsets(float *audio_data, int *onsets, float *all_NAE_values, int num_samples, int T)
// {
//     float cumulative_sum[MAX_SAMPLES];
//     calculate_cumulative_sum(audio_data, cumulative_sum, num_samples);

//     int n = 0;
//     int tn = 0;
//     for (int t = 1; t < num_samples; t++)
//     {
//         // Calculate NAE using precalculated cumulative sum
//         float NAE_left = (t - T > 0) ? calculate_NAE(cumulative_sum, tn, t - T) : cumulative_sum[t];
//         float NAE_middle = calculate_NAE(cumulative_sum, tn, t);
//         float NAE_right = (t + T < num_samples) ? calculate_NAE(cumulative_sum, tn, t + T) : cumulative_sum[t];

//         // Check condition for note onset
//         if (NAE_left > NAE_middle && NAE_right > NAE_middle)
//         {
//             onsets[n++] = t;
//             tn = t + T;
//         }

//         // Save NAE value for current time t
//         all_NAE_values[t] = NAE_middle;
//     }
// }

// WAV file header struct
typedef struct
{
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} WavHeader;

// // Function to calculate cumulative sum of an array
// void calculate_cumulative_sum(uint16_t *audio_data, float *cumulative_sum, int num_samples)
// {
//     cumulative_sum[0] = audio_data[0];
//     for (int i = 1; i < num_samples; i++)
//     {
//         cumulative_sum[i] = cumulative_sum[i - 1] + audio_data[i];
//     }
// }

// // Function to calculate Note Average Energy (NAE)
// float calculate_NAE(float *cumulative_sum, int tn, int t)
// {
//     if (t == tn)
//     {
//         return cumulative_sum[t];
//     }
//     return (cumulative_sum[t] - cumulative_sum[tn]) / (t - tn);
// }

// // Function to detect note onsets based on the power envelope
// void detect_note_onsets(uint16_t *audio_data, int num_samples, int T)
// {
//     // Allocate memory for cumulative sum
//     float *cumulative_sum = (float *)malloc(num_samples * sizeof(float));
//     if (cumulative_sum == NULL)
//     {
//         printf("Memory allocation failed\n");
//         return;
//     }

//     // Rectify the audio data
//     for (int i = 0; i < num_samples; i++)
//     {
//         audio_data[i] = fabs(audio_data[i]);
//     }

//     // Calculate cumulative sum
//     calculate_cumulative_sum(audio_data, cumulative_sum, num_samples);

//     // Initialize variables
//     int *onsets = (int *)malloc(num_samples * sizeof(int));
//     int n = 0;
//     int tn = 0; // Initial onset time

//     // Loop through each sample
//     for (int t = 1; t < num_samples; t++)
//     {
//         // Calculate NAE for left, middle, and right windows
//         float NAE_left = (t - T > 0) ? calculate_NAE(cumulative_sum, tn, t - T) : cumulative_sum[t];
//         float NAE_middle = calculate_NAE(cumulative_sum, tn, t);
//         float NAE_right = (t + T < num_samples) ? calculate_NAE(cumulative_sum, tn, t + T) : cumulative_sum[t];

//         // Check condition for note onset
//         if (NAE_left > NAE_middle && NAE_right > NAE_middle)
//         {
//             onsets[n++] = t;
//             tn = t + T; // Update onset time
//         }
//     }

//     // Print number of onsets
//     printf("Number of note onsets: %d\n", n);

//     // Free allocated memory
//     free(cumulative_sum);
//     free(onsets);
// }

// Function to calculate Note Average Energy (NAE)
float calculate_NAE(uint16_t *amplitude_envelope, int tn, int t, int T)
{
    if (t == tn)
    {
        return amplitude_envelope[t];
    }

    float integral_value = 0.0f;
    for (int i = tn; i < t; i++)
    {
        integral_value += amplitude_envelope[i];
    }

    return integral_value / (float)(t - tn);
}

// Function to detect note onsets based on the amplitude envelope
void detect_note_onsets(uint16_t *amplitude_envelope, int num_samples, int T)
{
    // Initialize variables
    int *onsets = (int *)malloc(num_samples * sizeof(int));
    int n = 0;
    int tn = 0; // Initial onset time

    // Store NAE values
    float *all_NAE_values = (float *)malloc(num_samples * sizeof(float));
    if (all_NAE_values == NULL || onsets == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    for (int t = 1; t < num_samples; t++)
    {
        // Calculate NAE for left, middle, and right windows
        float NAE_left = (t - T > 0) ? calculate_NAE(amplitude_envelope, tn, t - T, T) : amplitude_envelope[t];
        float NAE_middle = calculate_NAE(amplitude_envelope, tn, t, T);
        float NAE_right = (t + T < num_samples) ? calculate_NAE(amplitude_envelope, tn, t + T, T) : amplitude_envelope[t];

        // Check condition for note onset
        if (NAE_left > NAE_middle && NAE_right > NAE_middle)
        {
            onsets[n++] = t;
            tn = t + T; // Update onset time
        }

        // Save NAE value for current time t
        all_NAE_values[t] = NAE_middle;
    }

    printf("Number of note onsets: %d\n", n);

    // Free allocated memory
    free(onsets);
    free(all_NAE_values);
}

int main()
{
    // Read WAV file header and audio data
    // (You can use your provided code for reading WAV files here)
    FILE *file;
    WavHeader header;
    uint16_t *audio_data;
    int num_samples;

    // Open the WAV file
    file = fopen("C_major.wav", "rb");
    if (!file)
    {
        printf("Error opening file\n");
        return 1;
    }

    // Read the WAV file header
    fread(&header, sizeof(WavHeader), 1, file);

    // Allocate memory for audio data
    num_samples = header.subchunk2_size / (header.bits_per_sample / 8);
    audio_data = (uint16_t *)malloc(num_samples * sizeof(uint16_t));

    // Read audio data from the file
    fread(audio_data, sizeof(uint16_t), num_samples, file);

    // Close the file
    fclose(file);

    // Process the audio data as needed
    printf("NUM CHANNELS: %d\n", header.num_channels);
    printf("SAMPLE RATE: %d\n", header.sample_rate);
    printf("BYTE RATE: %d\n", header.byte_rate);
    printf("BITS PER SAMPLE: %d\n", header.bits_per_sample);
    printf("NUM SAMPLES: %d\n", num_samples);

    // Rectify the audio data
    for (int i = 0; i < num_samples; i++)
    {
        audio_data[i] = fabs(audio_data[i]);
    }

    // Define parameters
    int T = 5000; // Example value of T

    // Detect note onsets
    // detect_note_onsets(audio_data, num_samples, T);

    float test = calculate_NAE(audio_data, 110000, 111000, 5000);
    printf("%f\n", test);

    // Free allocated memory
    free(audio_data);

    return 0;
}
