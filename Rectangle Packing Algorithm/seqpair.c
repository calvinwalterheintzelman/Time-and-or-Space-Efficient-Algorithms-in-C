#include "seqpair.h"

void get_input(FILE* input, unsigned int* n, rect** rectangles)
{
	fseek(input, 0, SEEK_SET);
	char checker = fgetc(input);
	while(checker != '\n')
	{
		*n = (*n)*10 + checker - '0';
		checker = fgetc(input);
	}
	//gets number of rectangles ^^^
	
	*rectangles = malloc(sizeof(**rectangles) * (*n + 2));
	(*rectangles)[0].wid = 0; //source node
	(*rectangles)[0].len = 0;
	(*rectangles)[*n + 1].wid = 0; //sink node
	(*rectangles)[*n + 1].len = 0;

	for(unsigned int i = 1; i <= *n; i++)
	{
		for(unsigned int j = i; j != 0; j /= 10)
		{
			fgetc(input);
		}
		fgetc(input);
		double width = fgetc(input) - '0';
		fgetc(input);
		for(int j = 0; j < 6; j++)
		{
			width = width + ((double)(fgetc(input) - '0'))/10.0;
		}
		fgetc(input);
		fgetc(input);
		int exp = (fgetc(input) - '0')*10 + fgetc(input) - '0';
		while(exp != 0)
		{
			width *= 10.0;
			--exp;
		}
		fgetc(input);
		double length = fgetc(input) - '0';
		fgetc(input);
		for(int j = 0; j < 6; j++)
		{
			length = length + ((double)(fgetc(input) - '0'))/10.0;
		}
		fgetc(input);
		fgetc(input);
		exp = (fgetc(input) - '0')*10 + fgetc(input) - '0';
		while(exp != 0)
		{
			length *= 10.0;
			--exp;
		}
		fgetc(input);
		fgetc(input);

		(*rectangles)[i].wid = width;
		(*rectangles)[i].len = length;
	}
	//properly initializes array of rectangles ^^^
	
	unsigned int term;
	for(unsigned int i = 1; i <= *n; i++)
	{
		checker = fgetc(input);
		for(term = 0; checker != ' ' && checker != '\n'; checker = fgetc(input))
		{
			term = term*10 + (checker - '0');
		}
		(*rectangles)[term].idx1 = i;
	}
	for(unsigned int i = 1; i <= *n; i++)
	{
		checker = fgetc(input);
		for(term = 0; checker != ' ' && checker != '\n'; checker = fgetc(input))
		{
			term = term*10 + (checker - '0');
		}
		(*rectangles)[term].idx2 = i;
	}
	//gets arrays sp with index values (index in sp + 1 is the rectangle identifier) ^^^
	
	(*rectangles)[0].rights = create_node(1);
	node* curr = (*rectangles)[0].rights;
	for(unsigned int i = 2; i <= *n; i++)
	{
		curr->next = create_node(i);
		curr = curr->next;
	}
	curr->next = NULL;
	(*rectangles)[0].aboves = create_node(1);
	curr = (*rectangles)[0].aboves;
	for(unsigned int i = 2; i <= *n; i++)
	{
		curr->next = create_node(i);
		curr = curr->next;
	}
	curr->next = NULL;
	(*rectangles)[*n + 1].rights = NULL;
	(*rectangles)[*n + 1].aboves = NULL;
}


node* create_node(unsigned int index)
{
	node* new_node = malloc(sizeof(*new_node));
	new_node->idx = index;
	return new_node;
}


void pack(unsigned int n, rect* rectangles)
{
	for(unsigned int i = 1; i <= n; i++)
	{
		rectangles[i].rights = create_node(n + 1);
		node* r_curr = rectangles[i].rights;
		rectangles[i].aboves = create_node(n + 1);
		node* a_curr = rectangles[i].aboves;
		for(unsigned int j = 1; j <= n; j++)
		{
			if(rectangles[j].idx2 > rectangles[i].idx2)
			{
				if(rectangles[j].idx1 > rectangles[i].idx1)
				{
					r_curr->next = create_node(j);
					r_curr = r_curr->next;
				}
				else
				{
					a_curr->next = create_node(j);
					a_curr = a_curr->next;
				}
			}
		}
		r_curr->next = NULL;
		a_curr->next = NULL;
	}
	//gets the picture in the proj5 pdf ^^^

	for(unsigned int i = 0; i <= n + 1; i++)
	{
		rectangles[i].visited_w = false;
		rectangles[i].visited_l = false;
		rectangles[i].x = 0;
		rectangles[i].y = 0;
	}

	node* toposort_w = NULL;
	visit_w(0, &toposort_w, rectangles);

	for(node* curr = toposort_w; curr != NULL; curr = curr->next)
	{
		for(node* curr2 = rectangles[curr->idx].rights; curr2 != NULL; curr2 = curr2->next)
		{
			if(rectangles[curr2->idx].x < rectangles[curr->idx].x + rectangles[curr->idx].wid)
			{
				rectangles[curr2->idx].x = rectangles[curr->idx].x + rectangles[curr->idx].wid;
			}
		}
	}
	node* toposort_l = NULL;
	visit_l(0, &toposort_l, rectangles);	
	for(node* curr = toposort_l; curr != NULL; curr = curr->next)
	{
		for(node* curr2 = rectangles[curr->idx].aboves; curr2 != NULL; curr2 = curr2->next)
		{
			if(rectangles[curr2->idx].y < rectangles[curr->idx].y + rectangles[curr->idx].len)
			{
				rectangles[curr2->idx].y = rectangles[curr->idx].y + rectangles[curr->idx].len;
			}
		}
	}
	//finds longest path which is also the coordinates

	node* new;
	for(node* curr = toposort_w; curr != NULL; curr = new)
	{
		new = curr->next;
		free(curr);
	}	
	for(node* curr = toposort_l; curr != NULL; curr = new)
	{
		new = curr->next;
		free(curr);
	}
	//free topologically sorted lists
}


void visit_w(unsigned int i, node** head, rect* rectangles)
{
	if(rectangles[i].visited_w == false)
	{
		for(node* curr = rectangles[i].rights; curr != NULL; curr = curr->next)
		{
			visit_w(curr->idx, head, rectangles);
		}
		rectangles[i].visited_w = true;
		node* new_head = malloc(sizeof(*new_head));
		new_head->idx = i;
		new_head->next = *head;
		*head = new_head;
	}
}


void visit_l(unsigned int i, node** head, rect* rectangles)
{
	if(rectangles[i].visited_l == false)
	{
		for(node* curr = rectangles[i].aboves; curr != NULL; curr = curr->next)
		{
			visit_l(curr->idx, head, rectangles);
		}
		rectangles[i].visited_l = true;
		node* new_head = malloc(sizeof(*new_head));
		new_head->idx = i;
		new_head->next = *head;
		*head = new_head;
	}
}


void get_output(FILE* output, unsigned int n, rect* rectangles)
{
	for(unsigned int i = 1; i <= n; i++)
	{
		fprintf(output, "%d(%le,%le)\n", i, rectangles[i].x, rectangles[i].y);
	}
}


void free_rect(rect* rectangles, unsigned int n)
{
	for(unsigned int i = 0; i <= n + 1; i++)
	{
		node* curr = rectangles[i].rights;
		while(curr != NULL)
		{
			node* temp = curr;
			curr = curr->next;
			free(temp);
		}
		curr = rectangles[i].aboves;
		while(curr != NULL)
		{
			node* temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
	free(rectangles);
}
