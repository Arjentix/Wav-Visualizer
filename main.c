/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
*/

/*
* This program parses file of .wav format and show all extracted information.
* It also calls to the accessory program for showing data graph.
* The accessory program should be passing in this program as an argument of flag '-c'.
* Usage example: ./wav_vizualizer music.wav -c ./audo_visualization
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ncurses.h>
#include "wav_reader.h"

int wav_fd;
wav_data_t wav;

/* void onexit(void) - function that will be calling when program will end up. */
void onexit(void) {
	(void) endwin();
	(void) close(wav_fd);
}

int main(int argc, char *argv[])
{
	int i;
	char cmd_to_show_data[128];

	/* Initialization */
	memset(&wav, 0, sizeof(wav));
	*cmd_to_show_data = '\0';
	atexit(onexit);

	/* Checking for parameters */
	if (argc < 2 || argc == 3) {
		printf("Usage: %s filename [-c cmd_to_show_data]\n", argv[0]);
		return EXIT_FAILURE;
	}
	if (argc == 4) {
		if (strcmp(argv[2], "-c") == 0) {
			strcpy(cmd_to_show_data, argv[3]);
		}
	}

	/* Preparing ncurses screen */
	if (initscr() == NULL) {
		perror("Can not initialize screen\n");
		return EXIT_FAILURE;
	}
	scrollok(stdscr, TRUE);

	/* Openning .wav file */
	wav_fd = open(argv[1], O_RDONLY);
	if (wav_fd == -1) {
		perror("Can not open file\n");
		return EXIT_FAILURE;
	}

	/* Getting info, printing and showing */
	read_wav(wav_fd, &wav);
	print_wav(wav);
	show_wav(wav, wav_fd, cmd_to_show_data);

	/* End of working */
	printw("Press any key to close\n");
	getch();

	return EXIT_SUCCESS;
}
