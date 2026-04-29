#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
	/* stego image info */
	char* src_image_fname;//=>stego image file name
	FILE* fptr_src_fname;//=>stego image file pointer

	/* secret file info */
	char* secret_output_fname;//=>secret file name
	FILE* fptr_secret;//=>secret file pointer
	int sec_extn_size;//=>secret file extention size
	int sec_file_size;//=>secret file size

}DecodeInfo;

/* Decoding Function prototype */

/* check operation type */
Status read_and_validate_decode_args(char* argv[],DecodeInfo* decInfo);

/* do decoding */
Status do_decoding(DecodeInfo* decInfo);

/* file open */
Status open_image_file(DecodeInfo* decInfo);

/* skip header */
Status skip_bmp_header(DecodeInfo* decInfo);

/* decode magic string */
Status decode_magic_string(DecodeInfo* decInfo);

/* secret file extention size */
Status decode_secret_file_extn_size(DecodeInfo* decInfo);

/* secret file extention */
Status decode_secret_file_extn(DecodeInfo* decInfo);

/* secret file size */
Status decode_secret_file_size(DecodeInfo* decInfo);

/* secret file data */
Status decode_secret_file_data(DecodeInfo* decInfo);

/*  byte from lsb */
char decode_byte_from_lsb(char* buffer);

/* int from lsb */
int decode_int_from_lsb(char* buffer);

#endif
