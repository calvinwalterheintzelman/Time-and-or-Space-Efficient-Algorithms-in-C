#ifndef PACKING_H
#define PACKING_H
#include <stdio.h>
#include <stdbool.h>

typedef struct _leaf
{
	bool cut;
	int info;
	double width;
	double height;
	struct _leaf* left;
	struct _leaf* right;
} leaf;

typedef struct _coord
{
	double x;
	double y;
	int num;
	struct _coord* next;
} coord;

leaf* load_tree(char* checker, FILE* fp);
void free_tree(leaf* tree);
void print_tree(leaf* tree, FILE* fp);
void pack(leaf* tree, coord** list, double x, double y, coord** head);
void save_dimensions(FILE* fp, leaf* tree);
void save_coordinates(FILE* fp, coord* list);

#endif
