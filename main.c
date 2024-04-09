#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <sndfile.h>
#include "FFT/fft.h"
#include "fsm.h"

// Define the maximum number of note segments
#define MAX_SEGMENTS 100

// Define a struct to represent a note segment
typedef struct
{
    int start_index;
    int end_index;
} NoteSegment;

int main(int argc, char *argv[])
{
    // Read wav file
    SNDFILE *file;
    SF_INFO sfinfo;
    char *filepath;
    double *wav_arr;

    if (argc != 3)
    {
        printf("Usage: %s <filepath> <scale>\n", argv[0]);
        return 1;
    }
    filepath = argv[1];
    file = sf_open(filepath, SFM_READ, &sfinfo);
    if (!file)
    {
        printf("Error opening the file.\n");
        exit(1);
    }
    if (sfinfo.channels != 1)
    {
        printf("WAV file has %d channels. Only mono channel WAV files accepted.\n", sfinfo.channels);
        sf_close(file);
        exit(1);
    }
    sf_close(file);

    printf("No. frames: %ld\n", sfinfo.frames);
    printf("Samples Rate: %d\n", sfinfo.samplerate);

    // Declare an array of structs to store note segments
    NoteSegment segments[MAX_SEGMENTS];

    // get the note start and end
    segments[0].start_index = 50000;
    segments[0].end_index = 70000;

    segments[1].start_index = 80000;
    segments[1].end_index = 95000;

    segments[2].start_index = 115000;
    segments[2].end_index = 125000;

    segments[3].start_index = 150000;
    segments[3].end_index = 160000;

    segments[4].start_index = 180000;
    segments[4].end_index = 195000;

    segments[5].start_index = 215000;
    segments[5].end_index = 225000;

    segments[6].start_index = 250000;
    segments[6].end_index = 260000;

    segments[7].start_index = 280000;
    segments[7].end_index = 300000;

    wav_arr = wav_to_arr(filepath, sfinfo);

    // Set FSM to desired scale
    identifyScale(argv[2]);

    char *noteLogger;

    CombinedNoteFSM fsm = {STARTING_STATE,
                           noteLogger};

    // Perform FFT analysis on each segment
    for (int i = 0; i < MAX_SEGMENTS; i++)
    {
        // Extract segment from wav_arr
        int start_index = segments[i].start_index;
        int end_index = segments[i].end_index;
        int segment_length = end_index - start_index + 1;
        double segment[segment_length];
        double *freqArray = (double *)malloc(sizeof(double) * 5);
        for (int j = start_index; j <= end_index; j++)
        {
            segment[j - start_index] = wav_arr[j];
        }

        // Perform FFT analysis on the segment
        printf("Segment %d:\n", i + 1);
        freqArray = (double *)get_freq(freqArray, segment, segment_length, 44100); // Pass appropriate sample rate

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
        processScaleNote(&fsm, freqArray , scaleAddress);
    }

    // double freq = get_freq(wav_arr, sfinfo.frames, sfinfo.samplerate);
    // printf("Most common frequency: %lf Hz\n", freq);
    return 0;

    // Hardcode note start and end

    // Loop through note starts and ends

    //
}