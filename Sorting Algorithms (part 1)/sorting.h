#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>

typedef struct _Node {
	long value;
	struct _Node *next;
} Node;

long* Load_Into_Array(char* Filename, int* Size);
int Save_From_Array(char* Filename, long* Array, int Size);
void Shell_Sort_Array(long* Array, int Size, double* N_Cmp);

Node* Load_Into_List(char* Filename);
int Save_From_List(char* Filename, Node* list);
Node* Shell_Sort_List(Node* list, double *N_Cmp);

#endif
