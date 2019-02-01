/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
*/

#ifndef WAV_READER_H
#define WAV_READER_H

/*
* This file contains some structure and function declarations,
* which can be used to read, print and show data of wav file.
*/

#define DATA_LENGTH 256

/*
* This structure describes some titles in chunk named fmt.
* Fmt chunk should exist always, cause it contains important data.
*/
typedef struct fmt_data {
	short wFormatTag;		// Identifier of data format
	short nChannels;		// Number of channels
	long nSamplesPerSec;		// Number of samples in second
	long nAvgBytesPerSec;		// Number of bytes per second
	short nBlockAlign;		// Size of one block = (number of channels) * (bytes for one channel)
	short wBitsPerSample;		// Number of bits per sample
} fmt_data_t;

/*
* The follow strucure needs for containing data about artist, equipement, etc.
* INFO chunk mustn't exist always.
*/
typedef struct info_data {
	/* The location where the subject of the file is archived */
	char IARL[5];
	long IARL_size;
	char IARL_data[DATA_LENGTH];

	/* The artist of the original subject of the file */
	char IART[5];
	long IART_size;
	char IART_data[DATA_LENGTH];

	/* The name of the person or organization that commissioned the original subject of the file */
	char ICMS[5];
	long ICMS_size;
	char ICMS_data[DATA_LENGTH];

	/* General comments about the file or its subject */
	char ICMT[5];
	long ICMT_size;
	char ICMT_data[DATA_LENGTH];

	/* Copyright information about the file (e.g., "Copyright Some Company 2011") */
	char ICOP[5];
	long ICOP_size;
	char ICOP_data[DATA_LENGTH];

	/* The date the subject of the file was created (creation date) */
	char ICRD[5];
	long ICRD_size;
	char ICRD_data[DATA_LENGTH];

	/* Whether and how an image was cropped */
	char ICRP[5];
	long ICRP_size;
	char ICRP_data[DATA_LENGTH];

	/* The dimensions of the original subject of the file */
	char IDIM[5];
	long IDIM_size;
	char IDIM_data[DATA_LENGTH];

	/* Dots per inch settings used to digitize the file */
	char IDPI[5];
	long IDPI_size;
	char IDPI_data[DATA_LENGTH];

	/* The name of the engineer who worked on the file */
	char IENG[5];
	long IENG_size;
	char IENG_data[DATA_LENGTH];

	/* The genre of the subject */
	char IGNR[5];
	long IGNR_size;
	char IGNR_data[DATA_LENGTH];

	/* A list of keywords for the file or its subject */
	char IKEY[5];
	long IKEY_size;
	char IKEY_data[DATA_LENGTH];

	/* Lightness settings used to digitize the file */
	char ILGT[5];
	long ILGT_size;
	char ILGT_data[DATA_LENGTH];

	/* Medium for the original subject of the file */
	char IMED[5];
 	long IMED_size;
 	char IMED_data[DATA_LENGTH];

	/* Title of the subject of the file (name) */
	char INAM[5];
	long INAM_size;
	char INAM_data[DATA_LENGTH];

	/* The number of colors in the color palette used to digitize the file */
	char IPLT[5];
	long IPLT_size;
	char IPLT_data[DATA_LENGTH];

	/* Name of the title the subject was originally intended for */
	char IPRD[5];
	long IPRD_size;
	char IPRD_data[DATA_LENGTH];

	/* Description of the contents of the file (subject) */
	char ISBJ[5];
	long ISBJ_size;
	char ISBJ_data[DATA_LENGTH];

	/* Name of the software package used to create the file */
	char ISFT[5];
	long ISFT_size;
	char ISFT_data[DATA_LENGTH];

	/* The name of the person or organization that supplied the original subject of the file */
	char ISRC[5];
	long ISRC_size;
	char ISRC_data[DATA_LENGTH];

	/* The original form of the material that was digitized (source form) */
	char ISRF[5];
	long ISRF_size;
	char ISRF_data[DATA_LENGTH];
	
	/* The name of the technician who digitized the subject file */
	char ITCH[5];
	long ITCH_size;
	char ITCH_data[DATA_LENGTH];
} info_data_t;

/*
* The next structure exists just for better encapsulation.
* It describes LIST chunk, which can contain INFO subchunk.
* LIST chunk mustn't exist always.
*/
typedef struct list_data {
	char INFO[5];
	info_data_t info; 		// Content of info chunk
} list_data_t;

/*
* Main structure in this file. Contains all other structures as fields.
* Describes all wav file (except data chunk).
*/
typedef struct wav_data {
	char riff[5];			// Identifier of file format
	long filesize;			// General file size
	char rifftype[5];		// Riff data type
	char chunk_id1[5];		// Identifier of format description chunck
	long chunksize1;		// Size of format description chunk
	fmt_data_t fmt;			// Content of fmt chunk
	char chunk_id2[5];		// Identifier of 2nd chunk
	long chunksize2;		// Size of 2nd chunk
	list_data_t list;		// Content of list chunk
	char datachunk_id[5];		// Identifier of data chunk
	long datachunk_size;		// Size of data chunk
} wav_data_t;

/*
* void read_wav(int wav_fd, wav_data_t *wav) - reads data from file descriptor wav_fd
* and writes it to the wav (except data chunk).
*/
void read_wav(int wav_fd, wav_data_t *wav);

/*
* void print_wav(wav_data_t wav) - prints all information from wav to the screen.
* Before using this a read_wav function should be called for wav.
*/
void print_wav(wav_data_t wav);

/*
* void show_wav(wav_data_t wav, int wav_fd, char *cmd_to_show_data) - transfer data from wav_fd to
* another program, which should show the graph. Data delivers in format "%d %d %d...".
* All data is integers in range of [0; 2^15).
* Before using this function a read_wav function should be called for wav and wav_fd.
*/
void show_wav(wav_data_t wav, int wav_fd, char *cmd_to_show_data);

#endif // WAV_READER_H
