#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>
#include "FFT/fft.h"
#include "fsm/fsm.h"
#include "noteSegmentation/note_segmentation.h"

#define MAX_SEGMENTS 100

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
    FILE *outputFilePointer;

    outputFilePointer = fopen("output.txt", "a");
    if (outputFilePointer == NULL)
    {
        printf("Error creating txt file.\n");
        return 1;
    }

    if (argc != 3)
    {
        printf("Usage: %s <filepath> <scale>\n", argv[0]);
        fclose(outputFilePointer);
        return 1;
    }

    char *filepath = argv[1];

    FILE *file;
    WavHeader header;
    int16_t *audio_data;
    int num_samples;

    file = fopen(filepath, "rb");
    if (!file)
    {
        printf("Error opening file\n");
        fclose(outputFilePointer);
        return 1;
    }

    if (fread(&header, sizeof(WavHeader), 1, file) != 1)
    {
        printf("Error reading file header\n");
        fclose(file);
        fclose(outputFilePointer);
        return 1;
    }

    num_samples = header.subchunk2_size / (header.bits_per_sample / 8);
    printf("number of samples: %d\n", num_samples);

    audio_data = (int16_t *)malloc(num_samples * sizeof(int16_t));
    if (!audio_data)
    {
        printf("Error allocating memory for audio data\n");
        fclose(file);
        fclose(outputFilePointer);
        return 1;
    }

    if (fread(audio_data, sizeof(int16_t), num_samples, file) != num_samples)
    {
        printf("Error reading audio data\n");
        fclose(file);
        fclose(outputFilePointer);
        free(audio_data);
        return 1;
    }

    fclose(file);

    printf("NUM CHANNELS: %d\n", header.num_channels);
    printf("SAMPLE RATE: %d\n", header.sample_rate);
    printf("BYTE RATE: %d\n", header.byte_rate);
    printf("BITS PER SAMPLE: %d\n", header.bits_per_sample);
    printf("FORMAT: %d\n", header.audio_format);
    printf("NUM SAMPLES: %d\n", num_samples);

    NoteSegment *segments;
    int noOfSegments;

    segments = get_segments(audio_data, num_samples, &noOfSegments);

    identifyScale(argv[2]);

    CombinedNoteFSM fsm = {STARTING_STATE};

    for (int i = 0; i < noOfSegments; i++)
    {
        int start_index = segments[i].rise_index;
        int end_index = segments[i].fall_index;
        int segment_length = end_index - start_index + 1;
        double *segment = (double *)malloc(segment_length * sizeof(double));
        double *freqArray = (double *)malloc(5 * sizeof(double));

        if (!segment || !freqArray)
        {
            printf("Memory allocation failed\n");
            fclose(outputFilePointer);
            free(audio_data);
            free(segment);
            free(freqArray);
            return 1;
        }

        for (int j = start_index; j <= end_index; j++)
        {
            segment[j - start_index] = (double)audio_data[j];
        }

        printf("Segment %d:\n", i + 1);
        freqArray = get_freq(freqArray, segment, segment_length, 44100);

        for (int j = 0; j < 5; j++)
        {
            double freqIteration = freqArray[j];
            freqArray[j] = normalization(freqArray[j]);
        }

        processScaleNote(&fsm, freqArray, outputFilePointer);

        free(segment);
        free(freqArray);
    }

    if (fsm.currentNote != OCTAVE)
    {
        printf("SCALE INCOMPLETE\n");
    }

    fclose(outputFilePointer);
    free(audio_data);
    return 0;
}
