#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <sndfile.h>
#include "FFT/fft.h"
#include "fsm.h"
#include "noteSegmentation/note_segmentation.h"

// Define the maximum number of note segments
#define MAX_SEGMENTS 100

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

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <filepath> <scale>\n", argv[0]);
        return 1;
    }
    char *filepath = argv[1];

    // Read WAV file header and audio data
    // (You can use your provided code for reading WAV files here)
    FILE *file;
    WavHeader header;
    int16_t *audio_data;
    int num_samples;

    // Open the WAV file
    file = fopen(filepath, "rb");
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

    // Declare an array of structs to store note segments
    NoteSegment *segments;
    int noOfSegments;

    segments = get_segments(audio_data, num_samples, &noOfSegments);

    // Set FSM to desired scale
    identifyScale(argv[2]);

    char *noteLogger;

    CombinedNoteFSM fsm = {STARTING_STATE,
                           noteLogger};

    // Perform FFT analysis on each segment
    for (int i = 0; i < noOfSegments; i++)
    {
        // Extract segment from wav_arr
        int start_index = segments[i].rise_index;
        int end_index = segments[i].fall_index;
        int segment_length = end_index - start_index + 1;
        double segment[segment_length];
        double *freqArray = (double *)malloc(sizeof(double) * 5);
        for (int j = start_index; j <= end_index; j++)
        {
            segment[j - start_index] = (double)audio_data[j];
        }

        // Perform FFT analysis on the segment
        printf("Segment %d:\n", i + 1);
        freqArray = get_freq(freqArray, segment, segment_length, 44100); // Pass appropriate sample rate

        for (int j = 0; j < 5; j++)
        {
            // Loop through the possible frequencies for each segment
            double freqIteration = freqArray[j];
            // If its outside the boundary, it is invalid, change it to null
            if (freqIteration < 130 || freqIteration > 260)
            {
                freqArray[j] = 0;
            }
            // If within frequency boundary change to null
            else
            {
                freqArray[j] = normalization(freqArray[j]);
            }
        }

        // freqArray is now the array with normalized freq values
        processScaleNote(&fsm, freqArray);
    }

    if (fsm.currentNote != OCTAVE)
    {
        printf("SCALE INCOMPLETE\n");
    }

    // double freq = get_freq(wav_arr, sfinfo.frames, sfinfo.samplerate);
    // printf("Most common frequency: %lf Hz\n", freq);
    return 0;

    // Hardcode note start and end

    // Loop through note starts and ends

    //
}