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
    double *scaleArray;
} ScaleMapping;

double normalization(double freq);
void identifyScale(char *scaleInput);
void processScaleNote(CombinedNoteFSM *combinedFsm, double *frequency);
