/* NAME: ABINATH S
 * DATE: 17/11/2025
 * DESCRIPTION: To encode the secret data into a image file of .bmp format the encoding is the least significant bit image
 * 		stegnography method this is the file for encode data*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	//printf("width = %u\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	//printf("height = %u\n", height);

	// Return image capacity
	return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

/* function definition for open files */
Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	// Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}
	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}
	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}
	// No failure return e_success
	return e_success;
}
/* Operation type check */
OperationType check_operation_type(char *argv[])
{
	/* check for encoding or decoding */
	if(!(strcmp(argv[1],"-e")))
	{
		return e_encode;
	}
	else if(!(strcmp(argv[1],"-d")))
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}
/* read and validate arguments */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	/* evaluvate argument 2 ie, source image */
	char* arg2 = strstr(argv[2],".bmp");
	if(arg2)
	{
		if(!(strcmp(arg2,".bmp")))
		{
			encInfo->src_image_fname = argv[2];
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
	/* evaluate argument 3 ie, secret file */
	char* arg3;
	if(arg3 = strstr(argv[3],".txt"));
	else if(arg3 = strstr(argv[3],".c"));
	else if(arg3 = strstr(argv[3],".sh"));
	else
	{
		printf("Invalid format of secret file\n");
		return e_failure;
	}
	/* check for extention of secret file extention */
	if(!(strcmp(arg3,".txt")))
	{
		encInfo->secret_fname = argv[3];
		strcpy(encInfo->extn_secret_file,arg3);
	}
	else if(!(strcmp(arg3,".c")))
	{
		encInfo->secret_fname = argv[3];
		strcpy(encInfo->extn_secret_file,arg3);
	}
	else if(!(strcmp(arg3,".sh")))
	{
		encInfo->secret_fname = argv[3];
		strcpy(encInfo->extn_secret_file,arg3);
	}
	else
	{
		printf("Invalid format of secret file\n");
		return e_failure;
	}
	/* evaluate argument 4 is passed or not */
	if(argv[4] == NULL)
	{
		encInfo->stego_image_fname = "stego.bmp";
		return e_success;
	}
	else
	{
		char* arg4 = strstr(argv[4],".bmp");
		if(!(strcmp(arg4,".bmp")))
		{
			encInfo->stego_image_fname = argv[4];
			return e_success;
		}
		else
		{
			printf("Error: Invalid file for output image\n");
			return e_failure;
		}
	}
}
/* function definition for check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
	uint image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	uint magic_string_len = strlen(MAGIC_STRING);
	uint secret_extn_len = strlen(encInfo->extn_secret_file);
	uint secret_file_size = get_file_size(encInfo->fptr_secret);
	encInfo->size_secret_file = secret_file_size;
	uint encoding_things = 54 + (magic_string_len + 4 + secret_extn_len + 4 + secret_file_size)*8;
	if(encoding_things < image_capacity)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}
/* function definition for get file size */
uint get_file_size(FILE *fptr)
{
	fseek(fptr,0,SEEK_END);
	return ftell(fptr);
}
/* function definition for copy bmp header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	char buffer[54];
	rewind(fptr_src_image);
	fread(buffer,1,54,fptr_src_image);
	fwrite(buffer,1,54,fptr_dest_image);
	return e_success;
}
/* function definition for encode magic string */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	if(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		return e_success;
	}
}
/* function definition for encode data to image */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	data[size];
	for(int i=0;i<size;i++)
	{
		char buffer[8];
		fread(buffer,1,8,fptr_src_image);
		if(encode_byte_to_lsb(data[i],buffer) == e_success)
		{
			fwrite(buffer,1,8,fptr_stego_image);
		}
		else
		{
			return e_failure;
		}
	}
	return e_success;
}
/* function definition for encode byte to lsb */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
	int buffer = 0;
	for(int i=7;i>=0;i--)
	{
		image_buffer[buffer] = ((image_buffer[buffer] & (~1)) | ((data & (1<<i)) >> i));
		buffer++;
	}
	return e_success;
}
/* function definition for encode secret file extn size */
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
	char buffer[32];
	fread(buffer,1,32,encInfo->fptr_src_image);
	if(encode_int_to_lsb(size,buffer) == e_success)
	{
		fwrite(buffer,1,32,encInfo->fptr_stego_image);
	}
	else
	{
		return e_failure;
	}
	return e_success;
}
/* function definition for encode int to lsb */
Status encode_int_to_lsb(int data,char *image_buffer)
{
	int buffer = 0;
	for(int i=31;i>=0;i--)
	{
		image_buffer[buffer] = ((image_buffer[buffer] & (~1)) | ((data & (1<<i)) >> i));
		buffer++;
	}
	return e_success;
}
/* function definition for encode secret file extn*/
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
	if(encode_data_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		return e_success;
	}
	return e_failure;
}
/* function definition for encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
	char buffer[32];
	fread(buffer,1,32,encInfo->fptr_src_image);
	if(encode_int_to_lsb(file_size,buffer) == e_success)
	{
		fwrite(buffer,1,32,encInfo->fptr_stego_image);
	}
	else
	{
		return e_failure;
	}
	return e_success;
}/* function definition for encode secret data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char secret_file_data[encInfo->size_secret_file];
	rewind(encInfo->fptr_secret);
	fread(secret_file_data,1,encInfo->size_secret_file,encInfo->fptr_secret);
	if(encode_data_to_image(secret_file_data,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}
/* function definition for encode remaining data of the image */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char c;
	while(1)
	{
		fread(&c,1,1,fptr_src);
		if(feof(fptr_src))
		{
			break;
		}
		fwrite(&c,1,1,fptr_dest);
	}
	return e_success;
}
/* function definition for do encoding */
Status do_encoding(EncodeInfo *encInfo)
{
	/* open the source,secret and output files */
	if(open_files(encInfo) == e_success)
	{
		if(check_capacity(encInfo) == e_success)
		{
			if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
			{
				char* magic_str = MAGIC_STRING;
				if(encode_magic_string(magic_str,encInfo) == e_success)
				{
					if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
					{
						if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
						{
							if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
							{
								if(encode_secret_file_data(encInfo) == e_success)
								{
									if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
									{
										printf("Message encoded successfully\n");
										return e_success;
									}
									else
									{
										printf("Error: failed to copy remaining data\n");
										return e_failure;
									}
								}
								else
								{
									printf("Error: failed to store secret file data\n");
									return e_failure;
								}
							}
							else
							{
								printf("Error: failed to store secret file size\n");
								return e_failure;
							}
						}
						else
						{
							printf("Error: failed to store file extention\n");
							return e_failure;
						}
					}
					else
					{
						printf("Error: failed to store secret file extention size\n");
						return e_failure;
					}
				}
				else
				{
					printf("Error: failed to encode magic string\n");
					return e_failure;
				}
			}
			else
			{
				printf("Error: header failed to copy\n");
				return e_failure;
			}
		}
		else
		{
			printf("Error:The capacity of output is not enough for original file\n");
			return e_failure;
		}
	}
	else
	{
		printf("Error: Files cannot open\n");
		return e_failure;
	}
}
