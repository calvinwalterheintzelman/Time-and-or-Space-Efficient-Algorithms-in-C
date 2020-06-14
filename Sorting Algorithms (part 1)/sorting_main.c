#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(int argc, char** argv)
{
	clock_t start = clock();
	if(argc != 4)
	{
		fprintf(stderr, "\nError: Incorrect number of arguments.\n");
		return EXIT_FAILURE;
	}
	char* input = argv[2];
	char* output = argv[3];
	if(argv[1][0] == '-')
	{
		if(argv[1][1] == 'a' && argv[1][2] == '\0')
		{
			int Size;
			double N_Cmp;
			long* long_array = Load_Into_Array(input, &Size);

			clock_t arr_sort_start = clock();
			Shell_Sort_Array(long_array, Size, &N_Cmp);
			clock_t arr_sort_end = clock();

			Save_From_Array(output, long_array, Size);
			free(long_array);
			clock_t end = clock();

			printf("\nI/O time: %le\nSorting time: %le\nNumber of comparisons: %le\n", (double)(end - start)/CLOCKS_PER_SEC, (double)(arr_sort_end - arr_sort_start)/CLOCKS_PER_SEC, N_Cmp);
		}
		else if(argv[1][1] == 'l' && argv[1][2] == '\0')
		{
			Node* list = Load_Into_List(input);
			double N_Cmp;

			clock_t list_sort_start = clock();
			list = Shell_Sort_List(list, &N_Cmp);
			clock_t list_sort_end = clock();

			Save_From_List(output, list);

			while(list != NULL)
			{
				Node* temp = list;
				list = list->next;
				free(temp);
			}

			clock_t end = clock();
			printf("\nI/O time: %le\nSorting time: %le\nNumber of comparisons: %le\n", (double)(end - start)/CLOCKS_PER_SEC, (double)(list_sort_end - list_sort_start)/CLOCKS_PER_SEC, N_Cmp);
		}
		else
		{
			fprintf(stderr, "\nError: first argument should either be '-a' or '-l'.\n");
			return EXIT_FAILURE;
		}
	}
	else
	{
		fprintf(stderr, "\nError: first argument should either be '-a' or '-l'.\n");
		return EXIT_FAILURE;
	}



	return EXIT_SUCCESS;
}
