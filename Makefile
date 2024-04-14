CC = gcc
CFLAGS = -lm
TARGET = main
INPUT_FILE_SUCCESS = samples/C_major.wav
INPUT_FILE_FAIL = samples/C_major_trunc.wav
SCALE = CMajor
OUTPUT = output.txt

# List of source files
SRCS = $(wildcard *.c) $(wildcard */*.c)

# List of object files
OBJS = $(SRCS:.c=.o)

# Compilation rule for object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Compilation rule for the target executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Run the program
run_succ: $(TARGET)
	./$(TARGET) $(INPUT_FILE_SUCCESS) $(SCALE)

# Run the program
run_fail: $(TARGET)
	./$(TARGET) $(INPUT_FILE_FAIL) $(SCALE)

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET) $(OUTPUT)
