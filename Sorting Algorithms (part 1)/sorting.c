#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"

long* Load_Into_Array(char* Filename, int* Size)
{
	long* array;

	FILE* fp = fopen(Filename, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file.\n");
		*Size = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long int file_size = ftell(fp) / sizeof(*array);
	*Size = (int)(file_size);
	if(*Size == 0)
	{
		fclose(fp);
		return NULL;
	}
	array = malloc(sizeof(*array)*(*Size));
	if(array == NULL)
	{
		fclose(fp);
		*Size = 0;
		fprintf(stderr, "\nError: could not allocate space for the array.\n");
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);
	int long_as_str_size = sizeof(*array);
	unsigned char* long_as_str = malloc(long_as_str_size);
	if(long_as_str == NULL)
	{
		free(array);
		fclose(fp);
		*Size = 0;
		fprintf(stderr, "\nError: could not allocate space for the array.\n");
		return NULL;
	}
	
	for(long int j = 0; j < *Size; j++)
	{
		for(int i = 0; i <  long_as_str_size; i++)
		{
			long_as_str[i] = fgetc(fp);
		}
		memcpy(&(array[j]), long_as_str, sizeof(*array));
	}


	free(long_as_str);
	fclose(fp);
	return array;
}

int Save_From_Array(char* Filename, long* Array, int Size)
{
	int long_ints_returned = 0;
	FILE* fp = fopen(Filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file\n");
		return long_ints_returned;
	}
	char byte;
	long int bytes_used = sizeof(*Array)*Size / sizeof(byte);

	for(long i = 0; i < bytes_used; i++)
	{
		memcpy(&byte, (char*)(Array) + i*sizeof(byte), sizeof(byte));
		if(fputc(byte, fp) == EOF)
		{
			fprintf(stderr, "\nError: could not write to file.\n");
			break;
		}
		if((i+1) % sizeof(*Array) == 0)
		{
			long_ints_returned += 1;
		}
	}

	fclose(fp);
	return long_ints_returned;
}

void Shell_Sort_Array(long *Array, int Size, double *N_Cmp)
{
	*N_Cmp = 0;
	long k = 1;
	while(k < Size)
	{
		k = k*3 + 1;
	}

	for(; k >= 1; k = (k - 1)/3)
	{
		for(long j = k; j < Size; j++)
		{
			long i = j;
			long temp = Array[j];
			if(i < k)
			{
				*N_Cmp += 1;
			}
			while(i >= k && Array[i-k] > temp)
			{
				Array[i] = Array[i - k];
				i -= k;
				if(i < k)
				{
					*N_Cmp += 1;
				}

			}
			Array[i] = temp;
		}
	}
}

Node* Load_Into_List(char *Filename)
{
	FILE* fp = fopen(Filename, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file for reading.\n");
		return NULL;
	}
	Node* list = NULL;
	fseek(fp, 0, SEEK_END);
	long int file_size = ftell(fp) / sizeof(list->value);
	fseek(fp, 0, SEEK_SET);
	char* copier = malloc(sizeof(list->value));
	if(copier == NULL)
	{
		fprintf(stderr, "\nError: could not allocate space for loading linked-list.\n");
		fclose(fp);
		return list;
	}

	Node* curr = list;
	Node* prev = list;
	for(long i = 0; i < file_size; i++)
	{
		prev = curr;
		curr = malloc(sizeof(*curr));
		if(curr == NULL)
		{
			fprintf(stderr, "\nError: could not allocate space for loading linked-list.\n");
			break;
		}
		for(long j = 0; j < sizeof(list->value); j++)
		{
			copier[j] = fgetc(fp);
		}
		memcpy(&curr->value, copier, sizeof(list->value));
		curr->next = NULL;
		if(i == 0)
		{
			list = curr;
		}
		else
		{
			prev->next = curr;
		}
	}//puts a long into a node every iteration

	fclose(fp);
	free(copier);
	return list;
}

int Save_From_List(char* Filename, Node* list)
{
	int long_ints_returned = 0;
	FILE* fp = fopen(Filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "\nError: could not open file\n");
		return long_ints_returned;
	}
	Node* curr = list;
	char byte;
	int i = 0;

	while(curr != NULL)
	{
		memcpy(&byte, (char*)(&curr->value) + i*sizeof(byte), sizeof(byte));
		if(fputc(byte, fp) == EOF)
		{
			fprintf(stderr, "\nError: could not write to file.\n");
			break;
		}
		if((i++ +1) % sizeof(curr->value) == 0)
		{
			long_ints_returned += 1;
			i = 0;
			curr = curr->next;
		}
	}

	fclose(fp);
	return long_ints_returned;
}

Node* Shell_Sort_List(Node* list, double *N_Cmp)
{
	long size = 0;
	Node* curr = list;
	while(curr != NULL)
	{
		size += 1;
		curr = curr->next;
	}//size is correct; I checked it

	*N_Cmp = 0;
	long k = 1;
	while(k < size)
	{
		k = k*3 + 1;
	}

	Node* new_list = list;
	Node* prev1;
	Node* temp1; // left
	Node* prev2;
	Node* temp2; //right
	Node* after1;
	Node* temp_prev;
	for(k = (k-1)/3; k >= 1; k = (k-1)/3)
	{
		for(long j = k; j < size; j++)
		{
			long i = j;

			prev1 = new_list;
			for(long l = 1; l < i - k; l++)
			{
				prev1 = prev1->next;
			}
			if(i - k == 0)
			{
				temp1 = prev1; //when it's the head
			}
			else
			{
				temp1 = prev1->next;
			}

			prev2 = temp1;
			for(long l = 1; l < k; l++)
			{
				prev2 = prev2->next;
			}
			temp2 = prev2->next;
			
			if(i >= k)
			{
				*N_Cmp += 1;
			}
			while(i >= k && temp1->value > temp2->value)
			{
				if(k == 1)
				{
					temp1 ->next = temp2->next;
					temp2 ->next = temp1;
					if(i - k == 0)
					{
						prev2 = temp2;
						new_list = temp2;
					}
					else
					{
						prev2 = prev1;
						prev2 -> next = temp2;
					}
					prev1 = temp2;
				} //special case when k == 1
				else
				{
					//swap temp1 and temp2
					after1 = temp1->next;
					prev2->next = temp1;
					temp1->next = temp2->next;
					temp2->next = after1;
					if(temp1 == prev1) //temp1 is the head
					{
						new_list = temp2;
						prev1 = prev2;
						prev2 = temp2;
					}
					else
					{
						prev1 -> next = temp2;
						temp_prev = prev1;
						prev1 = prev2;
						prev2 = temp_prev;
					}
				}
					
				//update temp2 = temp1, find new temp1
				i -= k;
				if(i >= k)
				{
					prev1 = new_list;
					for(long l = 1; l < i - k; l++)
					{
						prev1 = prev1->next;
					}
					if(i - k == 0)
					{
						temp1 = prev1;
					}
					else
					{
						temp1 = prev1->next;
					}
					*N_Cmp += 1;
				}
				//update temp2 = temp1, find new temp1
			}
		}
	} //dont delete this bracket!

	return new_list;
}
