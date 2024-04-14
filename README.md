# bytebass

we make sure your fonky basslines are in the correct scale

**Compilation Command:**
```bash
gcc main.c FFT/fft.c fsm.c noteSegmentation/note_segmentation.c -o main -lm
```

**To run:**
```bash
./main samples/C_major.wav CMajor
./main samples/C_major_trunc.wav CMajor
```