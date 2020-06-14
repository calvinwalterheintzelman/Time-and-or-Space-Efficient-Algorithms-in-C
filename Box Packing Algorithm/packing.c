#include "packing.h"
#include <stdio.h>
#include <stdlib.h>

leaf* load_tree(char* checker, FILE* fp)
{
	leaf* node = malloc(sizeof(*node));
	if(node == NULL)
	{
		fprintf(stderr, "Error: could not allocate space.\n");
		return NULL;
	}
	if(*checker == 'H' || *checker == 'V')
	{
		bool horiz = false;
		if(*checker == 'H')
		{
			horiz = true;
		}
		node->cut = true;
		node->info = *checker;
		fgetc(fp);
		*checker = fgetc(fp);
		node->left = load_tree(checker, fp);
		node->right = load_tree(checker, fp);
		if(horiz == true)
		{
			if(node->left->width > node->right->width)
			{
				node->width = node->left->width;
			}
			else
			{
				node->width = node->right->width;
			}
			node->height = node->left->height + node->right->height;
		}
		else
		{
			if(node->left->height > node->right->height)
			{
				node->height = node->left->height;
			}
			else
			{
				node->height = node->right->height;
			}
			node->width = node->left->width + node->right->width;
		}
	}
	else
	{
		int info = *checker - 48;
		for(*checker = fgetc(fp); *checker != '('; *checker = fgetc(fp))
		{
			info = info*10 + *checker - 48;
		}
		node->cut = false;
		node->info = info;
		node->left = NULL;
		node->right = NULL;

		long width = fgetc(fp) - 48;
		fgetc(fp);
		for(int i = 0; i < 6; i++)
		{
			width = width*10 + fgetc(fp) - 48;
		}
		fgetc(fp);
		fgetc(fp);
		int exp = (fgetc(fp) - 48)*10 + fgetc(fp) - 48;
		while(6 - exp != 0)
		{
			width /= 10;
			exp++;
		}
		node->width = width;
		fgetc(fp);

		long height = fgetc(fp) - 48;
		fgetc(fp);
		for(int i = 0; i < 6; i++)
		{
			height = height*10 + fgetc(fp) - 48;
		}
		fgetc(fp);
		fgetc(fp);
		exp = (fgetc(fp) - 48)*10 + fgetc(fp) - 48;
		while(6 - exp != 0)
		{
			height /= 10;
			exp++;
		}
		node->height = height;
		fgetc(fp);
		fgetc(fp);
		*checker = fgetc(fp);
	}
	
	return node;
}

void free_tree(leaf* tree)
{
	if(tree->left != NULL)
	{
		free_tree(tree->left);
	}
	if(tree->right != NULL)
	{
		free_tree(tree->right);
	}
	free(tree);
}

void print_tree(leaf* tree, FILE* fp)
{
	if(tree->left != NULL)
	{
		print_tree(tree->left, fp);
	}
	if(tree->right != NULL)
	{
		print_tree(tree->right, fp);
	}

	if(tree->cut == true)
	{
		fprintf(fp, "%c\n", tree->info);
	}
	else
	{
		fprintf(fp, "%d(%le,%le)\n", tree->info, tree->width, tree->height);
	}
}

void pack(leaf* tree, coord** list, double x, double y, coord** head)
{
	if(tree->cut == true)
	{
		if(tree->info == 'H')
		{
			pack(tree->left, list, x, y + tree->right->height, head);
			pack(tree->right, list, x, y, head);
		}
		else
		{
			pack(tree->left, list, x, y, head);
			pack(tree->right, list, x + tree->left->width, y, head);
		}
	}
	else
	{
		coord* box = malloc(sizeof(*box));
		if(box == NULL)
		{
			fprintf(stderr, "Error: could not allocate space.\n");
			return;
		}
		box->x = x;
		box->y = y;
		box->num = tree->info;
		box->next = NULL;
		if(*list == NULL)
		{
			*list = box;
			*head = box;
		}
		else
		{
			(*list)->next = box;
			*list = box;
		}
	}
}

void save_dimensions(FILE* fp, leaf* tree)
{
	if(tree->cut == true)
	{
		fprintf(fp, "%c", tree->info);
	}
	else
	{
		fprintf(fp, "%d", tree->info); 
	}
	fprintf(fp, "(%le,%le)\n", tree->width, tree->height);

	if(tree->cut == true)
	{
		save_dimensions(fp, tree->left);
		save_dimensions(fp, tree->right);
	}
}

void save_coordinates(FILE* fp, coord* list)
{
	while(list != NULL)
	{
		fprintf(fp, "%d(%le,%le)\n", list->num, list->x, list->y);
		list = list->next;
	}
}
