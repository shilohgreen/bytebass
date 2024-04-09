#include <stdint.h>

double calculate_power(uint16_t *audio_data, int t_n, int t);
int detect_onsets(uint16_t *audio_data, int T, int audio_length);
