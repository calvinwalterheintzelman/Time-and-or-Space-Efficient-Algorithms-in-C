#include "seqpair.h"

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		return EXIT_FAILURE;
	}
	clock_t start = clock();
	char* input_name = argv[1];
	char* output_name = argv[2];

	unsigned int n = 0; 
	rect* rectangles;
	FILE* input = fopen(input_name, "r");
	if(input == NULL)
	{
		return EXIT_FAILURE;
	}
	get_input(input, &n, &rectangles);
	fclose(input);

	pack(n, rectangles);

	FILE* output = fopen(output_name, "w");
	if(output == NULL)
	{
		return EXIT_FAILURE;
	}
	get_output(output, n, rectangles);
	fclose(output);

	free_rect(rectangles, n);
	clock_t end = clock();

	printf("\nTotal time: %.2lf seconds.\n", ((double)(end - start))/(double)(CLOCKS_PER_SEC));

	return EXIT_SUCCESS;
}
