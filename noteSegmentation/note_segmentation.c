#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "note_segmentation.h"

#define MAX_SEGMENTS 100 // Maximum number of note segments

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
            // printf("Rise at %d with amplitude: %f\n", rise, moving_average[rise]);
            // printf("Fall at %d with amplitude: %f\n", fall, moving_average[rise]);
            rise = -1;
        }
    }

    printf("No. of onsets detected: %d\n", *segment_count);

    return segments;
}

NoteSegment *get_segments(int16_t *audio_data, int num_samples, int *segment_count)
{
    float threshold = 1000.0;
    int smoothing_window = 10000;

    // Rectify audio data
    int *rectified_audio_data = rectify(audio_data, num_samples);

    // Calculate moving average
    float *moving_average = calculate_moving_average(rectified_audio_data, num_samples, smoothing_window);

    // Detect note onsets
    NoteSegment *segments = detect_note_onsets(moving_average, num_samples, threshold, segment_count);

    return segments;
}