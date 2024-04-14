#include <stdio.h>
#include <string.h>

typedef enum
{
    STARTING_STATE,
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    FIFTH,
    SIXTH,
    SEVENTH,
    OCTAVE,
    INVALID_STATE
} Scale;

typedef struct
{
    Scale currentNote;
} CombinedNoteFSM;

typedef struct ScaleMapping
{
    char *name;
    double *scaleArray;
} ScaleMapping;

typedef struct NoteMapping
{
    char *note;
    int frequency;
} NoteMapping;

//                          C    C#   D    D#   E    F    F#   G    G#   A    A#   B
double allFrequencies[] = {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 245};

double cMajor[] = {131, 147, 165, 175, 196, 220, 245, 131};
double csMajor[] = {139, 156, 175, 185, 208, 220, 233, 139};
double dMajor[] = {147, 165, 185, 196, 220, 245, 139, 147};
double dsMajor[] = {156, 175, 196, 208, 220, 233, 139, 156};
double eMajor[] = {165, 185, 208, 220, 233, 245, 147, 165};
double fMajor[] = {175, 196, 220, 233, 245, 139, 147, 175};
double fsMajor[] = {185, 208, 233, 245, 139, 147, 156, 185};
double gMajor[] = {196, 220, 245, 131, 147, 156, 165, 196};
double gsMajor[] = {208, 233, 245, 139, 156, 165, 175, 208};
double aMajor[] = {220, 245, 131, 147, 156, 165, 185, 220};
double asMajor[] = {233, 245, 139, 156, 165, 175, 185, 233};
double bMajor[] = {245, 131, 147, 156, 165, 175, 196, 245};

double cMinor[] = {131, 147, 156, 175, 196, 208, 233, 131};
double csMinor[] = {139, 156, 165, 185, 208, 220, 245, 139};
double dMinor[] = {147, 165, 175, 196, 220, 233, 139, 147};
double dsMinor[] = {156, 175, 185, 208, 233, 245, 139, 156};
double eMinor[] = {165, 185, 196, 220, 245, 131, 147, 165};
double fMinor[] = {175, 196, 208, 233, 245, 139, 156, 175};
double fsMinor[] = {185, 208, 220, 245, 131, 147, 156, 185};
double gMinor[] = {196, 220, 233, 245, 139, 156, 165, 196};
double gsMinor[] = {208, 233, 245, 131, 147, 156, 165, 208};
double aMinor[] = {220, 245, 139, 156, 165, 175, 185, 220};
double asMinor[] = {233, 245, 139, 147, 156, 165, 175, 233};
double bMinor[] = {245, 131, 147, 156, 165, 175, 185, 245};

double *scaleAddress;

ScaleMapping scales[] = {
    {"cMajor", cMajor},
    {"cMinor", cMinor},
    {"csMajor", csMajor},
    {"csMinor", csMinor},
    {"dMajor", dMajor},
    {"dMinor", dMinor},
    {"dsMajor", dsMajor},
    {"dsMinor", dsMinor},
    {"eMajor", eMajor},
    {"eMinor", eMinor},
    {"fMajor", fMajor},
    {"fMinor", fMinor},
    {"fsMajor", fsMajor},
    {"fsMinor", fsMinor},
    {"gMajor", gMajor},
    {"gMinor", gMinor},
    {"gsMajor", gsMajor},
    {"gsMinor", gsMinor},
    {"aMajor", aMajor},
    {"aMinor", aMinor},
    {"asMajor", asMajor},
    {"asMinor", asMinor},
    {"bMajor", bMajor},
    {"bMinor", bMinor},
};

NoteMapping notes[] = {
    {"C", 131},
    {"C#", 139},
    {"D", 147},
    {"D#", 156},
    {"E", 165},
    {"F", 175},
    {"F#", 185},
    {"G", 196},
    {"G#", 208},
    {"A", 220},
    {"A#", 233},
    {"B", 245},
};
// This function normalizes a freq to the nearest note (represented by discrete frequency value within the allFrequencies array)
double normalization(double freq)
{
    int i;
    double diffA;
    double diffB;

    // round to nearest whole frequency
    for (i = 11; i >= 0; i--)
    {
        // Loop through frequency array to find it lies
        if (freq > allFrequencies[i])
        {
            // if its higher than B just round to B
            if (freq > 245)
            {
                return 245;
            }

            // it lies between allFrequencies[i] and allFrequencies[i + 1]
            diffA = freq - allFrequencies[i];
            diffB = allFrequencies[i + 1] - freq;

            if (diffA < diffB)
            {
                return allFrequencies[i];
            }
            else
            {
                return allFrequencies[i + 1];
            }
        }
    }

    return 131;
}

// from user input, load correct scale address from hash map
void identifyScale(char *scaleInput)
{
    int i;
    for (i = 0; i < 24; i++)
    {
        if (strcmp(scaleInput, scales[i].name))
        {
            scaleAddress = scales[i].scaleArray;
            break;
        }
    }
}

char *frequencyToNote(int freq)
{
    int i;
    for (i = 0; i < 12; i++)
    {
        if (freq == notes[i].frequency)
        {
            return notes[i].note;
        }
    }
    return notes[0].note;
}

void processScaleNote(CombinedNoteFSM *combinedFsm, double *frequency, double *modelScale, FILE *outputFilePointer)
{
    int i;
    switch (combinedFsm->currentNote)
    {
    case STARTING_STATE:
        // For each state, loop through the frequency array and check if the target freq is within
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[0])
            {
                combinedFsm->currentNote = FIRST;
                break;
            }
        }
        if (combinedFsm->currentNote == FIRST)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case FIRST:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[1])
            {
                combinedFsm->currentNote = SECOND;
                break;
            }
        }
        if (combinedFsm->currentNote == SECOND)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case SECOND:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[2])
            {
                combinedFsm->currentNote = THIRD;
                break;
            }
        }
        if (combinedFsm->currentNote == THIRD)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case THIRD:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[3])
            {
                combinedFsm->currentNote = FOURTH;
                break;
            }
        }
        if (combinedFsm->currentNote == FOURTH)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case FOURTH:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[4])
            {
                combinedFsm->currentNote = FIFTH;
                break;
            }
        }
        if (combinedFsm->currentNote == FIFTH)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case FIFTH:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[5])
            {
                combinedFsm->currentNote = SIXTH;
                break;
            }
        }
        if (combinedFsm->currentNote == SIXTH)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case SIXTH:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[6])
            {
                combinedFsm->currentNote = SEVENTH;
                break;
            }
        }
        if (combinedFsm->currentNote == SEVENTH)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;

    case SEVENTH:
        for (i = 0; i < 5; i++)
        {
            if (frequency[i] == scaleAddress[0])
            {
                combinedFsm->currentNote = OCTAVE;
                break;
            }
        }
        if (combinedFsm->currentNote == OCTAVE)
        {
            break;
        }
        combinedFsm->currentNote = INVALID_STATE;
        break;
    case OCTAVE:
        break;
    case INVALID_STATE:
        break;
    }
    printf("FSM State: %d\n", combinedFsm->currentNote);
    // Write state to file
    fprintf(outputFilePointer, " %s\n", frequencyToNote(frequency[i]));

    if (combinedFsm->currentNote == OCTAVE)
    {
        printf("SCALE COMPLETE\n");
    }
}
