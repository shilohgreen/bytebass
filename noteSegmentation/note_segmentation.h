// Struct to store rise and fall indices for a note segment
typedef struct
{
    int rise_index;
    int fall_index;
} NoteSegment;

NoteSegment *get_segments(int16_t *audio_data, int num_samples, int *segment_count);
