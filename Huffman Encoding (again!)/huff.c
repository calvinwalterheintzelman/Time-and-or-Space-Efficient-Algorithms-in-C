#include <stdio.h>
#include <stdlib.h>
#include "huff.h"
#include <string.h>
#include <stdbool.h>

bool huffman_code(char* comp_file, char* code_file, char* uncomp_file)
{
	FILE* comp = fopen(comp_file, "r");
	if(comp == NULL)
	{
		return false;
	}
	FILE* code = fopen(code_file, "w");
	if(code == NULL)
	{
		return false;
	}
	FILE* uncomp = fopen(uncomp_file, "w");
	if(uncomp == NULL)
	{
		return false;
	}
	fseek(comp, 0, SEEK_SET);
	long fgetc_counter = 0;
	
	long comp_chars;
	unsigned char* temp_array = malloc(sizeof(comp_chars));
	if(temp_array == NULL)
	{
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	for(long i = 0; i < sizeof(comp_chars); i++)
	{
		int checker = fgetc(comp);
		if(checker == EOF)
		{
			free(temp_array);
			fclose(comp);
			fclose(code);
			fclose(uncomp);
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = (unsigned char)(checker);
	}
	memcpy(&comp_chars, temp_array, sizeof(comp_chars));
	free(temp_array);

	long head_chars;
	temp_array = malloc(sizeof(head_chars));	
	if(temp_array == NULL)
	{
		free(temp_array);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	for(long i = 0; i < sizeof(head_chars); i++)
	{
		int checker = fgetc(comp);
		if(checker == EOF)
		{
			free(temp_array);
			fclose(comp);
			fclose(code);
			fclose(uncomp);
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = (unsigned char)(checker);
	}
	memcpy(&head_chars, temp_array, sizeof(head_chars));
	free(temp_array);

	long orig_chars;
	temp_array = malloc(sizeof(orig_chars));
	if(temp_array == NULL)
	{
		free(temp_array);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	for(long i = 0; i< sizeof(orig_chars); i++)
	{
		int checker = fgetc(comp);
		if(checker == EOF)
		{
			free(temp_array);
			fclose(comp);
			fclose(code);
			fclose(uncomp);
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = (unsigned char)(checker);
	}
	memcpy(&orig_chars, temp_array, sizeof(orig_chars));
	free(temp_array);

	unsigned char* header = malloc(sizeof(*header) * (head_chars - 1 ));
	if(header == NULL)
	{
		return false;
	}
	for(long i = 0; i < head_chars - 1; i++)
	{
		int ch = fgetc(comp);
		if(ch == EOF)
		{
			free(header);
			fclose(comp);
			fclose(code);
			fclose(uncomp);
			return false;
		}
		fgetc_counter += 1;
		header[i] = ch;
	}

	//printf("\nhead_chars is %ld\n", head_chars);

	for(long i = 0; i < head_chars - 1; i++)
	{
	//	printf("%c", header[i]);
	}

	if(fgetc(comp) != '0')
	{
		free(header);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	fgetc_counter += 1;

	long index = head_chars - 2;
	leaf* char_leaf = header_to_tree(header, &index, NULL);
	if(char_leaf == NULL)
	{
		free(header);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}

	long depth = find_depth(char_leaf, 0);

	unsigned char* binary_str = malloc(sizeof(*binary_str) * depth);
	if(binary_str == NULL)
	{
		free(header);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	print_code(char_leaf, code, 0, binary_str);

	long bit_str_length = comp_chars - head_chars - (long)(3*sizeof(comp_chars));
	unsigned char* bit_str = malloc(sizeof(*bit_str) * bit_str_length);
	if(bit_str == NULL)
	{
		free(header);
		free(binary_str);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}

	long i;
	for(i = 0; i < bit_str_length; i++)
	{
		int ch = fgetc(comp);
		if(ch == EOF)
		{
			free(header);
			free(binary_str);
			fclose(comp);
			fclose(code);
			fclose(uncomp);
			return false;
		}
		bit_str[i] = (unsigned char)(ch);
		fgetc_counter += 1;
	}
	unsigned char error_check = bit_str[i - 1];
	error_check = error_check << 7;
	error_check = error_check >> 7;
	if(error_check != '\0')
	{
		free(header);
		free(binary_str);
		fclose(comp);
		fclose(code);
		fclose(uncomp);
		return false;
	}
	
	long str_index = 0;

	bool success = print_orig(uncomp, char_leaf, &orig_chars, &str_index, bit_str);
	if(success == true)
	{
		printf("hi!");
	}
	else
	{
		printf("dlkfjd");
	}


	if(fgetc_counter != comp_chars)
	{
		success = false;
	}
	if(fgetc(comp) != EOF)
	{
		success = false;
	}

	free(binary_str);
	free(bit_str);
	free(header);
	free_tree(char_leaf);
	if(fclose(comp) == EOF)
	{
		success = false;
	}
	if(fclose(code) == EOF)
	{
		success = false;
	}
	if(fclose(uncomp) == EOF)
	{
		success = false;
	}

	return success; // should be success
}

bool print_orig(FILE* uncomp, leaf* char_leaf, long* orig_chars, long* str_index, unsigned char* bit_str)
{
	int bit_index = 0;
	while(*orig_chars > 0)
	{
		leaf* traversal = char_leaf;
		while(traversal->filled == false)
		{
			unsigned char bit_ch = bit_str[*str_index];
			bit_ch = bit_ch << bit_index;
			bit_ch = bit_ch >> 7;
			bit_index += 1;
			if(bit_index == 8)
			{
				bit_index = 0;
				*str_index += 1;
			}
			if(bit_ch == '\0' && traversal->left!=NULL)
			{
				traversal = traversal->left;
				if(traversal == NULL)
				{
					return false;
				}
			}
			else
			{
				if(bit_ch != '\0'+1)
				{
					return false;
				}
				traversal = traversal->right;
				if(traversal == NULL)
				{
					return false;
				}
			}
		}
		*orig_chars -= 1;
		fputc(traversal->ascii , uncomp);
	}

	while(bit_index < 8)
	{
		unsigned char error_check = bit_str[*str_index];
		error_check = error_check << bit_index;
		error_check = error_check >> 7;
		if(error_check != '\0')
		{
			return false;
		}
		bit_index += 1;
	}

	return true;
}

long find_depth(leaf* tree, long depth)
{
	long depth1 = 0;
	long depth2 = 0;
	if(tree->left != NULL)
	{
		depth1 = find_depth(tree->left, depth + 1);
	}
	if(tree->right != NULL)
	{
		depth2 = find_depth(tree->right, depth + 1);
	}
	if(tree->right == NULL && NULL == tree->left)
	{
		return depth;
	}

	if(depth1 >= depth2)
	{
		return depth1;
	}
	else
	{
		return depth2;
	}

}

void free_tree(leaf* tree)
{
	if(tree->left != NULL)
	{
		leaf* left_branch = tree->left;
		free_tree(left_branch);
	}
	if(tree->right != NULL)
	{
		leaf* right_branch = tree->right;
		free_tree(right_branch); 
	}
	free(tree);
}

void print_code(leaf* char_leaf, FILE* code, long root_depth, unsigned char* binary_str)
{
	if(char_leaf->filled == false)
	{
		long new_depth = root_depth + 1;

		if(char_leaf->left != NULL)
		{
			binary_str[root_depth] = '0';
			print_code(char_leaf->left, code, new_depth, binary_str);
		}
		if(char_leaf->right != NULL)
		{
			binary_str[root_depth] = '1';
			print_code(char_leaf->right, code, new_depth, binary_str);
		}
	}
	else
	{
		fputc(char_leaf->ascii, code);
		fputc(':', code);
		for(long i = 0; i < root_depth; i++)
		{
			fputc(binary_str[i], code);
		}
		fputc('\n', code);
	}
}

		for(long i = 0; i < root_depth; i++)
		{
			fputc(binary_str[i], code);
		}
		fputc('\n', code);
	}
}

leaf* header_to_tree(unsigned char* header, long* index, leaf* char_leaf)
{
	if(*index < 0)
	{
		return NULL;
	}
	leaf* root = malloc(sizeof(*root));
	if(root == NULL)
	{
		return NULL;
	}
	root->prev = char_leaf;
	if((header[*index] == '0' && *index - 1 >= 0 && header[*index - 1] != '1') || (*index - 2 >= 0 && header[*index - 1] == '1' && header[*index - 2] == '1'))
	{
		root->filled = false;
		*index -= 1;
		root->right = header_to_tree(header, index, root);
		root->left = header_to_tree(header, index, root);
	}
	else
	{
		root->filled = true;
		root->ascii = header[*index];
		root->left = NULL;
		root->right = NULL;
		printf("\n%c", header[*index]);
		*index -= 2;
	}

	return root;
}

bool huffman_bits(char* bit_file, char* uncomp_file)
{
	FILE* bits = fopen(bit_file, "r");
	if(bits == NULL)
	{
		return false;
	}
	FILE* uncomp = fopen(uncomp_file, "w");
	if(uncomp == NULL)
	{
		fclose(bits);
		return false;
	}
	long fgetc_counter = 0;

	long bit_chars;
	unsigned char* temp_array = malloc(sizeof(bit_chars));
	if(temp_array == NULL)
	{
		return false;
	}
	for(long i = 0; i < sizeof(bit_chars); i++)
	{
		int checker = fgetc(bits);
		if(checker == EOF)
		{
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = checker;
	}
	memcpy(&bit_chars, temp_array, sizeof(bit_chars));
	free(temp_array);

	long head_chars;
	temp_array = malloc(sizeof(head_chars));	
	if(temp_array == NULL)
	{
		return false;
	}
	for(long i = 0; i < sizeof(head_chars); i++)
	{		
		int checker = fgetc(bits);
		if(checker == EOF)
		{
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = checker;
	}
	memcpy(&head_chars, temp_array, sizeof(head_chars));
	free(temp_array);

	long orig_chars;
	temp_array = malloc(sizeof(orig_chars));
	if(temp_array == NULL)
	{
		return false;
	}
	for(long i = 0; i< sizeof(orig_chars); i++)
	{
		int checker = fgetc(bits);
		if(checker == EOF)
		{
			return false;
		}
		fgetc_counter += 1;
		temp_array[i] = (unsigned char)(checker);
	}
	memcpy(&orig_chars, temp_array, sizeof(orig_chars));
	free(temp_array);

	unsigned char* header = malloc(sizeof(*header) * head_chars);
	if(header == NULL)
	{
		return false;
	}
	for(long i = 0; i < head_chars; i++)
	{
		int checker = fgetc(bits);
		if(checker == EOF)
		{
			return false;
		}
		fgetc_counter += 1;
		header[i] = (unsigned char)(checker);
	}
	unsigned char* char_header = malloc(0);

	int bit_index = 0;
	long header_index = 0;
	long char_header_index = 0;
	while(header_index < head_chars)
	{
		unsigned char bin_bit = header[header_index] << bit_index;
		bin_bit = bin_bit >> 7;
		if(bin_bit == '\0')
		{
			char_header_index += 1;
			char_header = realloc(char_header, char_header_index);
			if(char_header == NULL)
			{
				return false;
			}
			char_header[char_header_index - 1] = '0';
			bit_index += 1;
			if(bit_index == 8)
			{
				bit_index = 0;
				header_index += 1;
			}
		}
		else
		{
			char_header_index += 2;
			char_header = realloc(char_header, char_header_index);
			if(char_header == NULL)
			{
				return false;
			}
			char_header[char_header_index - 2] = '1';
			bit_index += 1;
			if(bit_index == 8)
			{
				bit_index = 0;
				header_index += 1;
			}
	char_header_index -= 1;
	leaf* char_leaf = header_to_tree(char_header, &char_header_index, NULL);
	if(char_leaf == NULL)
	{
		return false;
	}

	while(char_leaf->left == NULL)
	{
		leaf* temp = char_leaf;
		char_leaf= char_leaf->right;
		if(char_leaf == NULL)
		{
			return false;
		}
		free(temp);
	}

	long bit_str_length = bit_chars - head_chars - (long)(3*sizeof(bit_chars));
	unsigned char* bit_str = malloc(sizeof(*bit_str) * bit_str_length);
	if(bit_str == NULL)
	{
		return false;
	}

	long i;
	for(i = 0; i < bit_str_length; i++)
	{
		int checker = fgetc(bits);
		if(checker == EOF)
		{
			return false;
		}
		fgetc_counter += 1;
		bit_str[i] = (unsigned char)(checker);
	}
	unsigned char error_check = bit_str[i - 1];
	error_check = error_check << 7;
	error_check = error_check >> 7;
	if(error_check != '\0')
	{
		return false;
	}

	long str_index = 0;

	bool success = print_orig(uncomp, char_leaf, &orig_chars, &str_index, bit_str);

	if(fgetc_counter != bit_chars)
	{
		success = false;
	}
	if(fgetc(bits) != EOF)
	{
		success = false;
	}

	free(bit_str);
	free_tree(char_leaf);
	free(header);
	free(char_header);
	if(fclose(bits) == EOF)
	{
		fclose(uncomp);
		success = false;
	}
	if(fclose(uncomp) == EOF)
	{
		success = false;
	}
	return success;
}
