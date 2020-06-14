#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huff.h"

int main(int argc, char** argv)
{
	if(argc == 5)
	{
		if(argv[1][0] == '-' && argv[1][1] == 'c' && argv[1][2] == '\0')
		{
			char* comp_file = argv[2];
			char* code_file = argv[3];
			char* uncomp_file = argv[4];
			if(comp_file == NULL || code_file == NULL || uncomp_file == NULL)
			{
				return EXIT_FAILURE;
			}
			bool success = huffman_code(comp_file, code_file, uncomp_file);
			if(success != true)
			{
				return EXIT_FAILURE;
			}
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	else if(argc == 4)
	{
		if(argv[1][0] == '-' && argv[1][1] == 'b' && argv[1][2] == '\0')
		{
			char* bit_file = argv[2];
			char* uncomp_file = argv[3];
			if(bit_file == NULL || uncomp_file == NULL)
			{
				return EXIT_FAILURE;
			}
			bool success = huffman_bits(bit_file, uncomp_file);
			if(success != true)
			{
				return EXIT_FAILURE;
			}
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	else
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
