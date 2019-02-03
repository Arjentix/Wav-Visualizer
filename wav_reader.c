/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
*/

/*
* This file contains definitions of functions declared in the wav_readed.h.
* All function descriptions (except read_infodata) you can find in wac_reader.h.
*/

#include "wav_reader.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <signal.h>

/* MAX_ATTEMPTS - means maximum attempts for finding data chunk, if it wans't */
#define MAX_ATTEMPTS 5

/*
* void read_infodata(int wav_fd, info_data_t *info_ptr, long info_size) - accessory function
* that reads data from INFO chunk of info_size size from file described by wav_fd
* and writes it to the info_ptr.
*/
void read_infodata(int wav_fd, info_data_t *info_ptr, long info_size);

void read_wav(int wav_fd, wav_data_t *wav_ptr)
{
	int sample_size;
	int num_of_sample;
	int i;

	memset(wav_ptr, 0, sizeof(wav_ptr));

	/* Start reading */
	read(wav_fd, wav_ptr->riff, 4);
	wav_ptr->riff[4] = '\0';

	read(wav_fd, &wav_ptr->filesize, 4);

	read(wav_fd, wav_ptr->rifftype, 4);
	wav_ptr->rifftype[4] = '\0';

	/* 1st chunk */
	read(wav_fd, wav_ptr->chunk_id1, 4);
	wav_ptr->chunk_id1[4] = '\0';

	read(wav_fd, &wav_ptr->chunksize1, 4);

	if (strcmp(wav_ptr->chunk_id1, "fmt ") == 0 ) {
		read(wav_fd, &wav_ptr->fmt.wFormatTag, 2);
		read(wav_fd, &wav_ptr->fmt.nChannels, 2);
		read(wav_fd, &wav_ptr->fmt.nSamplesPerSec, 4);
		read(wav_fd, &wav_ptr->fmt.nAvgBytesPerSec, 4);
		read(wav_fd, &wav_ptr->fmt.nBlockAlign, 2);
		read(wav_fd, &wav_ptr->fmt.wBitsPerSample, 2);
	}
	else {
		perror("No fmt chunk\n");
		exit(EXIT_FAILURE);
	}

	/* 2nd chunk */
	read(wav_fd, wav_ptr->chunk_id2, 4);
	wav_ptr->chunk_id2[4] = '\0';

	read(wav_fd, &wav_ptr->chunksize2, 4);

	if (strcmp(wav_ptr->chunk_id2, "LIST") == 0) {			// If it's a LIST chunk
		read(wav_fd, wav_ptr->list.INFO, 4);
		wav_ptr->list.INFO[4] = '\0';

		if (strncmp(wav_ptr->list.INFO, "INFO", 4) == 0) {	// If it contains INFO subchunk
			read_infodata(wav_fd, &wav_ptr->list.info, wav_ptr->chunksize2 - 4);
		}
		else {
			read(wav_fd, NULL, wav_ptr->chunksize2 - 4);	// Else passing bytes
		}
	}
	else if (strcmp(wav_ptr->chunk_id2, "data") == 0) {		// If it is a data chunk
		/* switching some data */
		strncpy(wav_ptr->datachunk_id, wav_ptr->chunk_id2, 5);
		wav_ptr->datachunk_size = wav_ptr->chunksize2;
		wav_ptr->chunk_id2[0] = '\0';
		wav_ptr->chunksize2 = 0;
	}
	else {
		read(wav_fd, NULL, wav_ptr->chunksize2);		// Passing bytes cause it is unknown chunk
	}

	/* Searching for the data chunk */
	for (i = 0; (i < MAX_ATTEMPTS) && (strncmp(wav_ptr->datachunk_id, "data", 4) != 0); i++) {
		read(wav_fd, wav_ptr->datachunk_id, 4);
		wav_ptr->datachunk_id[4] = '\0';

		read(wav_fd, &wav_ptr->datachunk_size, 4);

		if (strcmp(wav_ptr->datachunk_id, "data") != 0) {
			read(wav_fd, NULL, wav_ptr->datachunk_size);	// Passing bytes cause it is unknown chunk
		}
	}

	if (i == MAX_ATTEMPTS) {
		perror("Data chunk not found\n");
		exit(EXIT_FAILURE);
	}
}

void read_infodata(int wav_fd, info_data_t *info_ptr, long info_size)
{
	char id[5];
	long block_size;
	char *text;
	int i = 0;

	memset(info_ptr, 0, sizeof(info_ptr));

	while (i < info_size) {
		read(wav_fd, id, 4);
		i += 4;
		id[5] = '\0';

		read(wav_fd, &block_size, 4);
		i += 4;

		/* Alignment */
		if (block_size % 2 != 0) {
			block_size++;
		}

		text = (char*) malloc(block_size + 1);
		read(wav_fd, text, block_size);
		i += block_size;

		/*
		* Switch case analog for string.
		* Information about all id see in info_data_t declaration.
		*/
		if (strcmp(id, "IARL") == 0) {
			strncpy(info_ptr->IARL, id, 5);
			info_ptr->IARL_size = block_size;
			memcpy(info_ptr->IARL_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IART") == 0) {
			strncpy(info_ptr->IART, id, 5);
			info_ptr->IART_size = block_size;
			memcpy(info_ptr->IART_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ICMS") == 0) {
			strncpy(info_ptr->ICMS, id, 5);
			info_ptr->ICMS_size = block_size;
			memcpy(info_ptr->ICMS_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ICMT") == 0) {
			strncpy(info_ptr->ICMT, id, 5);
			info_ptr->ICMT_size = block_size;
			memcpy(info_ptr->ICMT_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ICOP") == 0) {
			strncpy(info_ptr->ICOP, id, 5);
			info_ptr->ICOP_size = block_size;
			memcpy(info_ptr->ICOP_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ICRD") == 0) {
			strncpy(info_ptr->ICRD, id, 5);
			info_ptr->ICRD_size = block_size;
			memcpy(info_ptr->ICRD_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ICRP") == 0) {
			strncpy(info_ptr->ICRP, id, 5);
			info_ptr->ICRP_size = block_size;
			memcpy(info_ptr->ICRP_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IDIM") == 0) {
			strncpy(info_ptr->IDIM, id, 5);
			info_ptr->IDIM_size = block_size;
			memcpy(info_ptr->IDIM_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IDPI") == 0) {
			strncpy(info_ptr->IDPI, id, 5);
			info_ptr->IDPI_size = block_size;
			memcpy(info_ptr->IDPI_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IENG") == 0) {
			strncpy(info_ptr->IENG, id, 5);
			info_ptr->IENG_size = block_size;
			memcpy(info_ptr->IENG_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IGNR") == 0) {
			strncpy(info_ptr->IGNR, id, 5);
			info_ptr->IGNR_size = block_size;
			memcpy(info_ptr->IGNR_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IKEY") == 0) {
			strncpy(info_ptr->IKEY, id, 5);
			info_ptr->IKEY_size = block_size;
			memcpy(info_ptr->IKEY_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ILGT") == 0) {
			strncpy(info_ptr->ILGT, id, 5);
			info_ptr->ILGT_size = block_size;
			memcpy(info_ptr->ILGT_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IMED") == 0) {
			strncpy(info_ptr->IMED, id, 5);
			info_ptr->IMED_size = block_size;
			memcpy(info_ptr->IMED_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "INAM") == 0) {
			strncpy(info_ptr->INAM, id, 5);
			info_ptr->INAM_size = block_size;
			memcpy(info_ptr->INAM_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IPLT") == 0) {
			strncpy(info_ptr->IPLT, id, 5);
			info_ptr->IPLT_size = block_size;
			memcpy(info_ptr->IPLT_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "IPRD") == 0) {
			strncpy(info_ptr->IPRD, id, 5);
			info_ptr->IPRD_size = block_size;
			memcpy(info_ptr->IPRD_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ISBJ") == 0) {
			strncpy(info_ptr->ISBJ, id, 5);
			info_ptr->ISBJ_size = block_size;
			memcpy(info_ptr->ISBJ_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ISFT") == 0) {
			strncpy(info_ptr->ISFT, id, 5);
			info_ptr->ISFT_size = block_size;
			memcpy(info_ptr->ISFT_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ISRC") == 0) {
			strncpy(info_ptr->ISRC, id, 5);
			info_ptr->ISRC_size = block_size;
			memcpy(info_ptr->ISRC_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ISRF") == 0) {
			strncpy(info_ptr->ISRF, id, 5);
			info_ptr->ISRF_size = block_size;
			memcpy(info_ptr->ISRF_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
		if (strcmp(id, "ITCH") == 0) {
			strncpy(info_ptr->ITCH, id, 5);
			info_ptr->ITCH_size = block_size;
			memcpy(info_ptr->ITCH_data, text, (block_size > DATA_LENGTH ? DATA_LENGTH : block_size));
			continue;
		}
	}
}

void print_wav(wav_data_t wav)
{
	/* Easy part */
	printw("Identifier of the file format: %s\n", wav.riff);
	printw("Size of the file: %ld\n", wav.filesize);
	printw("Type of RIFF: %4s\n", wav.rifftype);
	printw("Identifier of the format description block: %4s\n", wav.chunk_id1);
	printw("Size of the format description block: %ld\n", wav.chunksize1);
	printw("fmt chunk:\n");
	printw("\tIdentifier of the data format: %hd\n", wav.fmt.wFormatTag);
	printw("\tNumber of channels: %hd\n", wav.fmt.nChannels);
	printw("\tNumber of samples per second: %ld\n", wav.fmt.nSamplesPerSec);
	printw("\tAverage number of bytes per second: %ld\n", wav.fmt.nAvgBytesPerSec);
	printw("\tSize of one block: %hd\n", wav.fmt.nBlockAlign);
	printw("\tNumber of bits in one sample: %hd\n", wav.fmt.wBitsPerSample);

	/* Hard part */
	if (wav.chunk_id2[0] != '\0') {
		printw("Identifier of the 2nd chunk: %s\n", wav.chunk_id2);
		printw("Size of 2nd chunk: %ld\n", wav.chunksize2);

		if (strncmp(wav.chunk_id2, "LIST", 4) == 0) {
			printw("LIST chunk:\n");

			if (strncmp(wav.list.INFO, "INFO", 4) == 0) {
				printw("\tINFO subchunk:\n");

				/* Information about all id see in info_data_t declaration */
				if (wav.list.info.IARL[0] != '\0') {
					printw("\t\tThe location where the subject of the file is archived: %s\n", wav.list.info.IARL_data);
				}
				if (wav.list.info.IART[0] != '\0') {
					printw("\t\tThe artist of the original subject of the file: %s\n", wav.list.info.IART_data);
				}
				if (wav.list.info.ICMS[0] != '\0') {
					printw("\t\tThe name of the person or organization that commissioned the original subject of the file: %s\n", wav.list.info.ICMS_data);
				}
				if (wav.list.info.ICMT[0] != '\0') {
					printw("\t\tGeneral comments about the file or its subject: %s\n", wav.list.info.ICMT_data);
				}
				if (wav.list.info.ICOP[0] != '\0') {
					printw("\t\tCopyright information about the file (e.g., \"Copyright Some Company 2011\"): %s\n", wav.list.info.ICOP_data);
				}
				if (wav.list.info.ICRD[0] != '\0') {
					printw("\t\tThe date the subject of the file was created (creation date): %s\n", wav.list.info.ICRD_data);
				}
				if (wav.list.info.ICRP[0] != '\0') {
					printw("\t\tWhether and how an image was cropped: %s\n", wav.list.info.ICRP_data);
				}
				if (wav.list.info.IDIM[0] != '\0') {
					printw("\t\tThe dimensions of the original subject of the file: %s\n", wav.list.info.IDIM_data);
				}
				if (wav.list.info.IDPI[0] != '\0') {
					printw("\t\tDots per inch settings used to digitize the file: %s\n", wav.list.info.IDPI_data);
				}
				if (wav.list.info.IENG[0] != '\0') {
					printw("\t\tThe name of the engineer who worked on the file: %s\n", wav.list.info.IENG_data);
				}
				if (wav.list.info.IGNR[0] != '\0') {
					printw("\t\tThe genre of the subject: %s\n", wav.list.info.IGNR_data);
				}
				if (wav.list.info.IKEY[0] != '\0') {
					printw("\t\tA list of keywords for the file or its subject: %s\n", wav.list.info.IKEY_data);
				}
				if (wav.list.info.ILGT[0] != '\0') {
					printw("\t\tLightness settings used to digitize the file: %s\n", wav.list.info.ILGT_data);
				}
				if (wav.list.info.IMED[0] != '\0') {
					printw("\t\tMedium for the original subject of the file: %s\n", wav.list.info.IMED_data);
				}
				if (wav.list.info.INAM[0] != '\0') {
					printw("\t\tTitle of the subject of the file (name): %s\n", wav.list.info.INAM_data);
				}
				if (wav.list.info.IPLT[0] != '\0') {
					printw("\t\tThe number of colors in the color palette used to digitize the file: %s\n", wav.list.info.IPLT_data);
				}
				if (wav.list.info.IPRD[0] != '\0') {
					printw("\t\tName of the title the subject was originally intended for: %s\n", wav.list.info.IPRD_data);
				}
				if (wav.list.info.ISBJ[0] != '\0') {
					printw("\t\tDescription of the contents of the file (subject): %s\n", wav.list.info.ISBJ_data);
				}
				if (wav.list.info.ISFT[0] != '\0') {
					printw("\t\tName of the software package used to create the file: %s\n", wav.list.info.ISFT_data);
				}
				if (wav.list.info.ISRC[0] != '\0') {
					printw("\t\tThe name of the person or organization that supplied the original subject of the file: %s\n", wav.list.info.ISRC_data);
				}
				if (wav.list.info.ISRF[0] != '\0') {
					printw("\t\tThe original form of the material that was digitized (source form): %s\n", wav.list.info.ISRF_data);
				}
				if (wav.list.info.ITCH[0] != '\0') {
					printw("\t\tThe name of the technician who digitized the subject file: %s\n", wav.list.info.ITCH_data);
				}
			}
		}

		printw("Identifier of data chunk: %s\n", wav.datachunk_id);
		printw("Size of data chunk: %ld\n", wav.datachunk_size);
	}

	refresh();
}

void show_wav(wav_data_t wav, int wav_fd, char *cmd_to_show_data)
{
	FILE *showing_program;
	int sample_size;
	unsigned long i;
	unsigned short databuf;
	int res;

	/* If no program wasn't pass as an argument */
	if (*cmd_to_show_data == '\0') {
		printw("WARNING: Can't show graph. You need to run it with -c [cmd to show data]\n");
		return;
	}

	/*
	* Ignoring SIGPIPE signal so the program will not fall,
	* if another program for showing data will end up before the end of data transfer.
	*/
	signal(SIGPIPE, SIG_IGN);

	sample_size = wav.fmt.wBitsPerSample / 8;

	showing_program = popen(cmd_to_show_data, "w");
	if (showing_program == NULL) {
		perror("Failed to run showing program\n");
		exit(-1);
	}

	printw("Number of samples = %d\n", wav.datachunk_size / (wav.fmt.wBitsPerSample / 8));
	printw("Starting writing data to the showing program\n");
	refresh();

	for (i = 0; i < wav.datachunk_size / sample_size; i++) {
		read(wav_fd, &databuf , sample_size);
		fprintf(showing_program, "%d ", databuf);

		/* If showing program ended */
		if (ferror(showing_program) != 0) {
			printw("Stopping writing data to the showing program\n");
			refresh();
			break;
		}
	}

	res = pclose(showing_program);
	printw("Showing program ended with exit code: %d\n", res);
	refresh();
}
