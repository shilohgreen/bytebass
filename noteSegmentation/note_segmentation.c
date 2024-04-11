#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define MAX_SEGMENTS 100 // Maximum number of note segments

// Struct to store rise and fall indices for a note segment
typedef struct
{
    int rise_index;
    int fall_index;
} NoteSegment;

// Takes in raw audio data and rectifies all negative values.
int *rectify(int16_t *audio_data, int num_samples)
{
    int i;
    int *rectified_audio_data = (int *)malloc(sizeof(int) * num_samples);
    for (i = 0; i < num_samples; i++)
    {
        rectified_audio_data[i] = abs((int)audio_data[i]);
    }
    return rectified_audio_data;
}

// Returns an array with a moving average of rectified audio data provided smoothing window size.
float *calculate_moving_average(int *rectified_audio_data, int num_samples, int smoothing_window)
{
    int i;
    int sum = 0;
    float *moving_average = (float *)malloc(sizeof(float) * num_samples);
    for (i = 0; i < smoothing_window; i++)
    {
        sum += rectified_audio_data[i];
        moving_average[i] = rectified_audio_data[i];
    }

    for (i = smoothing_window; i < num_samples; i++)
    {
        sum += rectified_audio_data[i] - rectified_audio_data[i - smoothing_window];
        moving_average[i] = (float)sum / smoothing_window;
    }

    return moving_average;
}

// Detects note onsets
NoteSegment *detect_note_onsets(float *moving_average, int num_samples, float threshold, int *segment_count)
{

    int i;
    int rise = -1;
    int fall = -1;
    *segment_count = 0; // Count of detected note segments

    // Allocate memory for array of structs to store note segments
    NoteSegment *segments = (NoteSegment *)malloc(MAX_SEGMENTS * sizeof(NoteSegment));
    if (segments == NULL)
    {
        *segment_count = 0;
        return NULL;
    }

    for (i = 0; i < num_samples; i++)
    {
        if (moving_average[i] > threshold && rise == -1)
        {
            rise = i;
        }
        else if (moving_average[i] < threshold && rise != -1)
        {
            fall = i;
            // Store rise and fall indices in struct
            segments[*segment_count].rise_index = rise;
            segments[*segment_count].fall_index = fall;
            // Increment segment count
            (*segment_count)++;
            printf("Rise at %d with amplitude: %f\n", rise, moving_average[rise]);
            printf("Fall at %d with amplitude: %f\n", fall, moving_average[rise]);
            rise = -1;
        }
    }

    printf("No. of onsets detected: %d\n", *segment_count);

    return segments;
}

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

void main()
{
    // Read WAV file header and audio data
    // (You can use your provided code for reading WAV files here)
    FILE *file;
    WavHeader header;
    int16_t *audio_data;
    int num_samples;

    // Open the WAV file
    file = fopen("../samples/C_major3.wav", "rb");
    if (!file)
    {
        printf("Error opening file\n");
    }

    // Read the WAV file header
    fread(&header, sizeof(WavHeader), 1, file);

    // Allocate memory for audio data
    num_samples = header.subchunk2_size / (header.bits_per_sample / 8);
    audio_data = (int16_t *)malloc(num_samples * sizeof(int16_t));

    // Read audio data from the file
    fread(audio_data, sizeof(int16_t), num_samples, file);

    // Close the file
    fclose(file);

    // Process the audio data as needed
    printf("NUM CHANNELS: %d\n", header.num_channels);
    printf("SAMPLE RATE: %d\n", header.sample_rate);
    printf("BYTE RATE: %d\n", header.byte_rate);
    printf("BITS PER SAMPLE: %d\n", header.bits_per_sample);
    printf("FORMAT: %d\n", header.audio_format);
    printf("NUM SAMPLES: %d\n", num_samples);

    // Rectify audio data
    int *rectified_audio_data = rectify(audio_data, num_samples);

    // Declare parameters
    float threshold = 1000.0;
    int smoothing_window = 3000;
    int segment_count;

    // Calculate moving average
    float *moving_average = calculate_moving_average(rectified_audio_data, num_samples, smoothing_window);

    // Detect note onsets
    detect_note_onsets(moving_average, num_samples, threshold, &segment_count);
}