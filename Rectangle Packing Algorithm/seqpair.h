#ifndef SEQPAIR_H
#define SEQPAIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct _node
{
	unsigned int idx;
	struct _node* next;
} node;

typedef struct
{
	double wid; //width
	double len; //length
	node* rights;
	node* aboves;
	double x; //x-coordinate
	double y; //y-coordinate
	unsigned int idx1;
	unsigned int idx2;
	bool visited_w;
	bool visited_l;
} rect;

void get_input(FILE* fp, unsigned int* n, rect** rectangles);
void pack(unsigned int n, rect* rectangles);
node* create_node(unsigned int index);
void free_rect(rect* rectangles, unsigned int n);
void visit_w(unsigned int i, node** head, rect* rectangles);
void visit_l(unsigned int i, node** head, rect* rectangles);
void get_output(FILE* output, unsigned int n, rect* rectangles);


#endif
