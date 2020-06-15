#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

long* Load_Into_Array(char* Filename, int* Size);
int Save_From_Array(char* Filename, long* Array, int Size);

int main(int argc, char** argv)
{
	char* file = argv[2];
	int size;
	long* array = Load_Into_Array(file, &size);
	clock_t start = 0;
	clock_t end = 0;

	if(argv[1][0] == 'q')
	{
		start = clock();
		Quick_Sort(array, size);	
		end = clock();
	}

	if(argv[1][0] == 'm')
	{
		start = clock();
		Merge_Sort(array, size);
		end = clock();
	}

	/*for(long i = 0; i < size; i++)
	{
		printf("%ld\n", array[i]);
	}*/

	/*for(long i = 1; i < size; i++)
	{
		if(array[i - 1] > array[i])
		{
			printf("\nnot sorted!\n");
			break;
		}
	}*/

	printf("\nTotal Time: %le seconds.\n", (double)(end - start)/CLOCKS_PER_SEC);
	
	free(array);

	return EXIT_SUCCESS;
}


long* Load_Into_Array(char* Filename, int* Size)
{
	long* array;

	FILE* fp = fopen(Filename, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file.\n");
		*Size = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long int file_size = ftell(fp) / sizeof(*array);
	*Size = (int)(file_size);
	if(*Size == 0)
	{
		fclose(fp);
		return NULL;
	}
	array = malloc(sizeof(*array)*(*Size));
	if(array == NULL)
	{
		fclose(fp);
		*Size = 0;
		fprintf(stderr, "\nError: could not allocate space for the array.\n");
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);
	int long_as_str_size = sizeof(*array);
	unsigned char* long_as_str = malloc(long_as_str_size);
	if(long_as_str == NULL)
	{
		free(array);
		fclose(fp);
		*Size = 0;
		fprintf(stderr, "\nError: could not allocate space for the array.\n");
		return NULL;
	}
	
	for(long int j = 0; j < *Size; j++)
	{
		for(int i = 0; i <  long_as_str_size; i++)
		{
			long_as_str[i] = fgetc(fp);
		}
		memcpy(&(array[j]), long_as_str, sizeof(*array));
	}


	free(long_as_str);
	fclose(fp);
	return array;
}

int Save_From_Array(char* Filename, long* Array, int Size)
{
	int long_ints_returned = 0;
	FILE* fp = fopen(Filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file\n");
		return long_ints_returned;
	}
	char byte;
	long int bytes_used = sizeof(*Array)*Size / sizeof(byte);

	for(long i = 0; i < bytes_used; i++)
	{
		memcpy(&byte, (char*)(Array) + i*sizeof(byte), sizeof(byte));
		if((i+1) % sizeof(*Array) == 0)
		{
			long_ints_returned += 1;
		}
	}

	fclose(fp);
	return long_ints_returned;
}
