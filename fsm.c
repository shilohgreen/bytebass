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

void processMajorScaleNote(CombinedNoteFSM *combinedFsm, char note_char)
{
    switch (combinedFsm->currentNote)
    {
    case STARTING_STATE:
        if (note_char == "C")
        {
            combinedFsm->currentNote = FIRST;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIRST:
        if (note_char == "D")
        {
            combinedFsm->currentNote = SECOND;
        }
        else if (note_char == 'C')
        {
            combinedFsm->currentNote = FIRST;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SECOND:
        if (note_char == 'E')
        {
            combinedFsm->currentNote = THIRD;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case THIRD:
        if (note_char == 'F')
        {
            combinedFsm->currentNote = FOURTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FOURTH:
        if (note_char == 'G')
        {
            combinedFsm->currentNote = FIFTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIFTH:
        if (note_char == 'A')
        {
            combinedFsm->currentNote = SIXTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SIXTH:
        if (note_char == 'B')
        {
            combinedFsm->currentNote = SEVENTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;

    case SEVENTH:
        if (note_char == 'C')
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

void processMinorScaleNote(CombinedNoteFSM *combinedFsm, char note_char)
{
    switch (combinedFsm->currentNote)
    {
    case STARTING_STATE:
        if (note_char == "C")
        {
            combinedFsm->currentNote = FIRST;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIRST:
        if (note_char == "D")
        {
            combinedFsm->currentNote = SECOND;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SECOND:
        if (note_char == 'E-FLAT')
        {
            combinedFsm->currentNote = THIRD;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case THIRD:
        if (note_char == 'F')
        {
            combinedFsm->currentNote = FOURTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FOURTH:
        if (note_char == 'G')
        {
            combinedFsm->currentNote = FIFTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case FIFTH:
        if (note_char == 'A-FLAT')
        {
            combinedFsm->currentNote = SIXTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;
    case SIXTH:
        if (note_char == 'B-FLAT')
        {
            combinedFsm->currentNote = SEVENTH;
        }
        else
        {
            combinedFsm->currentNote = INVALID_STATE;
        }
        break;

    case SEVENTH:
        if (note_char == 'C')
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
    char *cMajorScale = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'C'};
    char *dMinorScale = {'D', 'E', 'F', 'G', 'A', 'B-FLAT', 'C', 'D'};
    return 0;
}
