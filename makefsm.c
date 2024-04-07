#include <stdio.h>
#include <string.h>
#include "makefsm.h"

int main(int argc, char** argv){
    char * root_note;
    char * scale;
    int i;
    int * scale_pos; 
    double target_freq[8];
    /*these are the fundamental freqiencies at C0*/
    /*the logic is intended to implement it in a way that the frequency from FFT will be gradually halved until it has reached 15-32Hz range*/
    /* then based on this, this produces the target frequencies to compare with the input audio*/
    double fundamental_freq[12] = {16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87};

    int C_major_pos[8] = {0, 2, 4, 5, 7, 9, 11, 0};
    int C_minor_pos[8] = {0, 2, 3, 5, 7, 8, 10, 0};

    root_note = argv[1];
    scale = argv[2];

    if (strcmp(scale, "major") ==0){
        scale_pos = offset_scale(root_note, C_major_pos, 8);
    }
    else if (strcmp(scale, "minor") ==0){
        scale_pos = offset_scale(root_note, C_minor_pos, 8);
    }

    if (strcmp(scale, "major") ==0){
        for (i=0; i<8; i++){
            target_freq[i] = fundamental_freq[scale_pos[i]];
        }
    }
    else if (strcmp(scale, "minor") ==0){
        for (i=0; i<8; i++){
            target_freq[i] = fundamental_freq[scale_pos[i]];
        }
    }
    else{
        printf("Invalid Input for scale\n!");
        return 0;
    }

    printf("target frequencies to detect: ");
    for (i=0; i<8; i++){
        printf("%f, ", target_freq[i]);
    }
    printf("\nend of makefsm\n");


    return 0;
}

int * offset_scale(char * root_note, int * scale, int scale_len){
    int i;
    int * out = scale;

    if (strcmp(root_note, "C")==0){
        return out;
    }
    else if (strcmp(root_note, "D")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +2) %12;
        }
    }
    else if (strcmp(root_note, "E")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +4) %12;
        }
    }
    else if (strcmp(root_note, "F")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +5) %12;
        }
    }
    else if (strcmp(root_note, "G")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +7) %12;
        }
    }
    else if (strcmp(root_note, "A")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +9) %12;
        }
    }
    else if (strcmp(root_note, "B")==0){
        for (i=0; i<scale_len; i++){
            out[i] = (scale[i] +11) %12;
        }
    }

    return out;
}