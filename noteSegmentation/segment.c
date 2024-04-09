#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "wav.h"

#define MAX_SEGMENTS 1000 // Maximum number of note segments

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

// Struct to store rise and fall indices for a note segment
typedef struct
{
    int rise_index;
    int fall_index;
} NoteSegment;

// Function to calculate moving average of audio data
void calculate_moving_average(uint16_t *audio_data, int num_samples, int window_size, float *moving_average)
{
    int sum = 0;
    for (int i = 0; i < window_size; i++)
    {
        sum += audio_data[i];
    }
    printf("sum: %d\n", sum);
    moving_average[window_size - 1] = (float)sum / window_size;

    for (int i = window_size; i < num_samples; i++)
    {
        sum -= audio_data[i - window_size];
        sum += audio_data[i];
        moving_average[i] = (float)sum / window_size;
    }
}

// Function to detect note segments (rise and fall points) on moving average
NoteSegment *detect_note_segments(uint16_t *audio_data, int num_samples, float threshold, int window_size, int *segment_count)
{
    // Allocate memory for array of structs to store note segments
    NoteSegment *segments = (NoteSegment *)malloc(MAX_SEGMENTS * sizeof(NoteSegment));
    if (segments == NULL)
    {
        *segment_count = 0;
        return NULL;
    }

    // Calculate moving average of audio data
    float *moving_average = (float *)malloc(num_samples * sizeof(float));
    calculate_moving_average(audio_data, num_samples, window_size, moving_average);

    for (int i = 45000; i < 80000; i++)
    {
        printf("%f  ", moving_average[i]);
    }

    // Initialize variables
    int rise_index = -1; // Index of the first rise above threshold
    int fall_index = -1; // Index of the first fall below threshold
    *segment_count = 0;  // Count of detected note segments

    // Loop through each sample
    for (int i = 0; i < num_samples; i++)
    {
        // Check for rise above threshold
        if (moving_average[i] > threshold && rise_index == -1)
        {
            // printf("RISE: %f @ %d\n", moving_average[i], i);
            rise_index = i;
        }
        // Check for fall below threshold
        if (moving_average[i] < threshold && audio_data[i] > 2000 && rise_index != -1 && fall_index == -1)
        {
            // printf("FALL: %f @ %d\n", moving_average[i], i);
            fall_index = i;
            // Store rise and fall indices in struct
            segments[*segment_count].rise_index = rise_index;
            segments[*segment_count].fall_index = fall_index;
            // Increment segment count
            (*segment_count)++;
            // Check if maximum segments reached
            if (*segment_count >= MAX_SEGMENTS)
            {
                break;
            }
            // Reset rise and fall indices
            rise_index = -1;
            fall_index = -1;
        }
    }

    free(moving_average); // Free allocated memory for moving average
    return segments;
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

    // parameters
    int window_size = 10000;
    float threshold = 1000.0; // Example threshold value

    // Pointer to array of structs to store note segments
    NoteSegment *segments;
    // Count of detected note segments
    int segment_count;

    // Detect note segments
    segments = detect_note_segments(audio_data, num_samples, threshold, window_size, &segment_count);

    // Print detected note segments
    printf("Detected note segments:\n");

    printf("No. of segments: %d\n", segment_count);
    // for (int i = 0; i < segment_count; i++)
    // {
    //     printf("Segment %d: Rise at index %d (%d), Fall at index %d (%d)\n", i + 1, segments[i].rise_index, audio_data[i], segments[i].fall_index, audio_data[i]);
    // }

    // Free allocated memory
    free(audio_data);
    free(segments);

    return 0;
}
