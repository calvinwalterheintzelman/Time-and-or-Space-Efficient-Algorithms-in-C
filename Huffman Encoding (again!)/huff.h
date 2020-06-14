#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdbool.h>

typedef struct _leaf
{
	unsigned char ascii;
	bool filled;
	struct _leaf* left;
	struct _leaf* right;
	struct _leaf* prev;
} leaf;

typedef struct _bin_char
{
	long int num_ascii;
	long int num_binary;
} bin_char;

bool huffman_code(char* comp_file, char* code_file, char* uncomp_file);
leaf* header_to_tree(unsigned char* header, long int* index, leaf* char_leaf);
void print_code(leaf* char_leaf, FILE* code, long int root_depth, unsigned char* binary_str);
void free_tree(leaf* tree);
long int find_depth(leaf* tree, long int depth);
bool print_orig(FILE* uncomp, leaf* char_leaf, long int* orig_chars, long int* str_index, unsigned char* bit_str);
bool huffman_bits(char* bit_file, char* uncomp_file);

#endif
