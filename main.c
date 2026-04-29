/* NAME: ABINATH S
 * DATE: 17/11/2025
 * DESCRIPTION; To perform lsb image stegnography this is the main function and checking commend line arguments.
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"
#include "common.h"

int main(int argc,char* argv[])
{
	EncodeInfo encInfo;
	if(!(argc > 3 && argc <= 5))
	{
		printf("Error: implicit declaration of arguments\n");
		return 0;
	}
	/* check operation type */
	OperationType type = check_operation_type(argv);
	if(type == e_encode)
	{
		/* read and validate arguments */
		if(read_and_validate_encode_args(argv,&encInfo) == e_success)
		{
			/* function call for deo encoding */
			if(do_encoding(&encInfo) == e_success);
		}
		else
		{
			printf("Error: validation commend line arguments failed\n");
		}
	}
	else if(type == e_decode)
	{
		DecodeInfo decInfo;
		/* read and validate arguments */
		if(read_and_validate_decode_args(argv,&decInfo) == e_success)
		{
			/* function call for do decoding */
			if(do_decoding(&decInfo) == e_success);
		}
		else
		{
			printf("Error: validating command line arguments failed\n");
		}
	}
	else
	{
		/* Error and help menu */
		printf("encode : ./a.out -e source_file.bmp secret_file.txt/.c/.sh\n");
		printf("decode : ./a.out -d stego_image.bmp output_secret\n");
	}
	return 0;
}
