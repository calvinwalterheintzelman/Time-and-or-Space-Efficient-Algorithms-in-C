#include <stdio.h>
#include <stdlib.h>
#include "packing.h"

int main(int argc, char** argv)
{
	if(argc != 5)
	{
		fprintf(stderr, "Error: incorrect number of inputs.\n");
		return EXIT_FAILURE;
	}
	char* input   = argv[1];
	char* output1 = argv[2];
	char* output2 = argv[3];
	char* output3 = argv[4];
	
	FILE* fp = fopen(input, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "Error: could not open input file.\n");
		return EXIT_FAILURE;
	}

	char checker;
	checker = fgetc(fp);
	leaf* tree = load_tree(&checker, fp);
	fclose(fp);

	fp = fopen(output1, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "Error: could not open first output file.\n");
		return EXIT_FAILURE;
	}
	print_tree(tree, fp);
	fclose(fp);

	coord* last = NULL;
	coord* head = NULL;
	pack(tree, &last, 0, 0, &head);

	fp = fopen(output2, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "Error: could not open second output file.\n");
		return EXIT_FAILURE;
	}
	save_dimensions(fp, tree);
	fclose(fp);

	fp = fopen(output3, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "Error: could not open third output file.\n");
		return EXIT_FAILURE;
	}
	save_coordinates(fp, head);
	fclose(fp);

	while(head != NULL)
	{
		last = head;
		head = head->next;
		free(last);
	}
	
	free_tree(tree);
	return EXIT_SUCCESS;
}
