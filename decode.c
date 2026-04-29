/*  NAME: ABINATH S
 *  DATE: 17/11/2025
 *  DESCRIPTION: To decode the encoded data from the image and store in the file with the exack formate
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

/* function definition for read and validate decode arguments */
Status read_and_validate_decode_args(char* argv[],DecodeInfo* decInfo)
{
	/* check argument 2 is.bmp or not */
	char* arg2 = strstr(argv[2],".bmp");
	if(arg2)
	{
		if(!(strcmp(arg2,".bmp")))
		{
			decInfo->src_image_fname = argv[2];
		}
		else
		{
			printf("Invalid formate of source image\n");
			return e_failure;
		}
	}
	else
	{
		printf("Invalid formate of source image\n");
		return e_failure;
	}
	/* check if argument 3 is passed or not */
	if(argv[3] != NULL)
	{
		decInfo->secret_output_fname = argv[3];
	}
	else
	{
		decInfo->secret_output_fname = NULL;
	}
	return e_success;
}
/* function deinition for open file */
Status open_image_file(DecodeInfo* decInfo)
{
	decInfo->fptr_src_fname = fopen(decInfo->src_image_fname, "r");
	if (decInfo->fptr_src_fname == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

		return e_failure;
	}
	return e_success;
}
/* function definition for skip header */
Status skip_bmp_header(DecodeInfo* decInfo)
{
	fseek(decInfo->fptr_src_fname,54,SEEK_CUR);
	return e_success;
}
/* function definition for decode magic string */
Status decode_magic_string(DecodeInfo* decInfo)
{
	int i;
	char magic_string[3];
	char buffer[8];
	for(i=0;i<2;i++)
	{
		fread(buffer,1,8,decInfo->fptr_src_fname);
		magic_string[i] = decode_byte_from_lsb(buffer);
	}
	magic_string[i] = '\0';
	if(strcmp(magic_string,MAGIC_STRING) == 0)
	{
		return e_success;
	}
	else
	{
		printf("Error: magic string not same\n");
		return e_failure;
	}
}
/* function definition from decode byte from lsb */
char decode_byte_from_lsb(char* buffer)
{
	char decoded_byte = 0;
	int j = 0;
	for(int i=7;i>=0;i--)
	{
		decoded_byte = decoded_byte | ((buffer[j] & 1) << i);
		j++;
	}
	return decoded_byte;
}
/* function definition for decode secret file extention size*/
Status decode_secret_file_extn_size(DecodeInfo* decInfo)
{
	char buffer[32];
	fread(buffer,1,32,decInfo->fptr_src_fname);
	decInfo->sec_extn_size = decode_int_from_lsb(buffer);
	return e_success;
}
/* function definition for decode int from lsb */
int decode_int_from_lsb(char* buffer)
{
	int i;
	int j = 0;
	int size = 0;
	for(i=31;i>=0;i--)
	{
		size = size | ((buffer[j] & 1) << i);
		j++;
	}
	return size;
}
/* function definition for secret file extention */
Status decode_secret_file_extn(DecodeInfo* decInfo)
{
	int i;
	char extn[decInfo->sec_extn_size + 1];
	for(i=0;i<decInfo->sec_extn_size;i++)
	{
		char buffer[8];
		fread(buffer,1,8,decInfo->fptr_src_fname);
		extn[i] = decode_byte_from_lsb(buffer);
	}
	extn[i] = '\0';
	if(decInfo->secret_output_fname == NULL)
	{
		static char output_sec[25] = "output_secret";
		strcat(output_sec,extn);
		decInfo->secret_output_fname = output_sec;
	}
	decInfo->fptr_secret = fopen(decInfo->secret_output_fname,"w");
	return e_success;
}
/* function definition for decode secret file size */
Status decode_secret_file_size(DecodeInfo* decInfo)
{
	char buffer[32];
	fread(buffer,1,32,decInfo->fptr_src_fname);
	decInfo->sec_file_size = decode_int_from_lsb(buffer);
	return e_success;
}
/* function definition for decode secret file data */
Status decode_secret_file_data(DecodeInfo* decInfo)
{
	for(int i=0;i<decInfo->sec_file_size;i++)
	{
		char buffer[8];
		fread(buffer,1,8,decInfo->fptr_src_fname);
		char ch = decode_byte_from_lsb(buffer);
		fwrite(&ch,1,1,decInfo->fptr_secret);
	}
	return e_success;
}
/* function definition for decoding */
Status do_decoding(DecodeInfo* decInfo)
{
	if(open_image_file(decInfo) == e_success)
	{
		if(skip_bmp_header(decInfo) == e_success)
		{
			if(decode_magic_string(decInfo) == e_success)
			{
				if(decode_secret_file_extn_size(decInfo) == e_success)
				{
					if(decode_secret_file_extn(decInfo) == e_success)
					{
						if(decode_secret_file_size(decInfo) == e_success)
						{
							if(decode_secret_file_data(decInfo) == e_success)
							{
								printf("Message decoded successfully\n");
								return e_success;
							}
							else
							{
								printf("Error: failed decode secret file data\n");
								return e_failure;
							}
						}
						else
						{
							printf("Error: failes decode secret file size\n");
							return e_failure;
						}
					}
					else
					{
						printf("Error: failed decode extention\n");
						return e_failure;
					}
				}
				else
				{
					printf("Error: failed decode extention size\n");
					return e_failure;
				}
			}
			else
			{
				printf("Error: failed decode magic string\n");
				return e_failure;
			}
		}
		else
		{
			printf("Error: failed in skip header\n");
			return e_failure;
		}
	}
	else
	{
		printf("Error: image file not opened\n");
		return e_success;
	}
}
