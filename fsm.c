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
    char *noteLogger;
} CombinedNoteFSM;

typedef struct ScaleMapping
{
    char *name;
    int *scaleArray;
} ScaleMapping;

//                      C    C#   D    D#   E    F    F#   G    G#   A    A#   B
int allFrequencies[] = {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 245};

int cMajor[] = {131, 147, 165, 175, 196, 220, 245, 131};
int csMajor[] = {139, 156, 175, 185, 208, 220, 233, 139};
int dMajor[] = {147, 165, 185, 196, 220, 245, 139, 147};
int dsMajor[] = {156, 175, 196, 208, 220, 233, 139, 156};
int eMajor[] = {165, 185, 208, 220, 233, 245, 147, 165};
int fMajor[] = {175, 196, 220, 233, 245, 139, 147, 175};
int fsMajor[] = {185, 208, 233, 245, 139, 147, 156, 185};
int gMajor[] = {196, 220, 245, 131, 147, 156, 165, 196};
int gsMajor[] = {208, 233, 245, 139, 156, 165, 175, 208};
int aMajor[] = {220, 245, 131, 147, 156, 165, 185, 220};
int asMajor[] = {233, 245, 139, 156, 165, 175, 185, 233};
int bMajor[] = {245, 131, 147, 156, 165, 175, 196, 245};

int cMinor[] = {131, 147, 156, 175, 196, 208, 233, 131};
int csMinor[] = {139, 156, 165, 185, 208, 220, 245, 139};
int dMinor[] = {147, 165, 175, 196, 220, 233, 139, 147};
int dsMinor[] = {156, 175, 185, 208, 233, 245, 139, 156};
int eMinor[] = {165, 185, 196, 220, 245, 131, 147, 165};
int fMinor[] = {175, 196, 208, 233, 245, 139, 156, 175};
int fsMinor[] = {185, 208, 220, 245, 131, 147, 156, 185};
int gMinor[] = {196, 220, 233, 245, 139, 156, 165, 196};
int gsMinor[] = {208, 233, 245, 131, 147, 156, 165, 208};
int aMinor[] = {220, 245, 139, 156, 165, 175, 185, 220};
int asMinor[] = {233, 245, 139, 147, 156, 165, 175, 233};
int bMinor[] = {245, 131, 147, 156, 165, 175, 185, 245};

int *scaleAddress;

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

// This function will first divide the frequencies by 2 until it is within the desired range
// After division, it will round to the nearest acceptable frequency (defined by all frequencies)
int normalization(int freq)
{
    int i;
    int diffA;
    int diffB;
    // 250 arbitrary cut off for 245Hz (B3)
    while (freq > 250)
    {
        freq = freq / 2;
    }

    // round to nearest whole frequency
    for (i = 23; i >= 0; i--)
    {
        // Loop through frequency array to find it lies
        if (freq > allFrequencies[i])
        {
            // if its higher than B3 just round to B3
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
}

// from user input, hash into array
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

void processScaleNote(CombinedNoteFSM *combinedFsm, int frequency, int *modelScale)
{
    switch (combinedFsm->currentNote)
    {
    case STARTING_STATE:
        if (frequency == scaleAddress[0])
        {
            combinedFsm->currentNote = FIRST;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIRST:
        if (frequency == scaleAddress[1])
        {
            combinedFsm->currentNote = SECOND;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SECOND:
        if (frequency == scaleAddress[2])
        {
            combinedFsm->currentNote = THIRD;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case THIRD:
        if (frequency == scaleAddress[3])
        {
            combinedFsm->currentNote = FOURTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FOURTH:
        if (frequency == scaleAddress[4])
        {
            combinedFsm->currentNote = FIFTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIFTH:
        if (frequency == scaleAddress[5])
        {
            combinedFsm->currentNote = SIXTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SIXTH:
        if (frequency == scaleAddress[6])
        {
            combinedFsm->currentNote = SEVENTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;

    case SEVENTH:
        if (frequency == scaleAddress[7])
        {
            combinedFsm->currentNote = OCTAVE;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case INVALID_STATE:
        break;
    }
}

int main()
{
    return 0;
}
