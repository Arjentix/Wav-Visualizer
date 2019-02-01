all: wav_visualizer

# Compiler
CC = gcc

# Linker flags
FLAGS = -lncurses

wav_visualizer: main.o wav_reader.o
	$(CC) -o wav_visualizer main.o wav_reader.o $(FLAGS)

main.o: main.c wav_reader.h
wav_reader.o: wav_reader.c wav_reader.h

clean:
	-rm main.o wav_reader.o
