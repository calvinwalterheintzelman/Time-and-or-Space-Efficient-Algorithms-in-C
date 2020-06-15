#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"

int partition(long* array, int lb, int ub, int* mid_right);
void q_sort(long* array, int lb, int ub);
void merge(long* array, long* temp, int left, int mid, int right);

void Merge_Sort(long *Array, int Size)
{
	//I use insertion sort for beginning small subarrays since it's faster
	long insert_sort_limit = 16;
	long ub = insert_sort_limit - 1;
	long lb;
	for(lb = 0; ub < Size; lb += insert_sort_limit, ub += insert_sort_limit)
	{
		for(long j = lb + 1; j <= ub; ++j)
		{
			for(long i = j; i >= lb + 1 && Array[i - 1] > Array[i]; --i)
			{
				long temp = Array[i - 1];
				Array[i - 1] = Array[i];
				Array[i] = temp;
			}
		}
	}
	for(long j = lb + 1; j < Size; ++j)
	{
		for(long i = j; Array[i - 1] > Array[i] && i >= lb + 1; --i)
		{
			long temp = Array[i - 1];
			Array[i - 1] = Array[i];
			Array[i] = temp;
		}
	}
	
	long ind; //index
	long merge_size = insert_sort_limit;
	long* temp = malloc(sizeof(*temp) * Size);
	while(merge_size < Size)
	{
		ind = 0;
		while(ind < Size - merge_size)
		{
			if(ind + 2*merge_size - 1 < Size - 1)
			{
				merge(Array, temp, ind, ind + merge_size - 1, ind + 2*merge_size - 1);
			}
			else
			{
				merge(Array, temp, ind, ind + merge_size - 1, Size - 1);
			}
			ind += 2*merge_size;
		}
		merge_size *= 2;
	}
	free(temp);
}

void merge(long* array, long* temp, int left, int mid, int right)
{
	memcpy(&temp[left], &array[left], (mid - left + 1)*sizeof(*array));
	memcpy(&temp[mid + 1], &array[mid + 1], (right - mid)*sizeof(*array));
	int i = left;
	int j = mid + 1;
	for(long k = left; k <= right; ++k)
	{
		if(i > mid)
		{
			array[k] = temp[j++];
		}
		else if(j > right)
		{
			array[k] = temp[i++];
		}
		else if(temp[j] < temp[i])
		{
			array[k] = temp[j++];
		}
		else
		{
			array[k] = temp[i++];
		}
	}
}

void Quick_Sort(long *Array, int Size)
{
	q_sort(Array, 0, Size - 1);
}

void q_sort(long* array, int lb, int ub)
{
	if(ub - lb > 30)
	{
		long down = lb;
		int right_part;
		long left_part;
		
		while(down < ub)
		{
			left_part = partition(array, down, ub, &right_part);
			q_sort(array, down, left_part);
			down = right_part;
		}
	}
	else //uses insertion sort when a subarray is 30 elements long or shorter to increase speed
	{
		for(long i = lb + 1; i <= ub; ++i)
		{
			for(long j = i; j > lb && array[j - 1] > array[j]; --j)
			{
				long temp = array[j - 1];
				array[j - 1] = array[j];
				array[j] = temp;
			}
		}
	}
}

int partition(long* array, int lb, int ub, int* mid_right) //returns highest index that contains values less than the pivot
//mid_right is the lowest index that contains values greater than the pivot
//everything in between these two indeces is equal to the pivot
{
	long mid = (ub +lb) / 2;
	long up = ub;
	long down = lb;
	long pivot;
	long dup_check = lb;
	long temp;

	//median of 3 for pivot
	if(array[mid] > array[lb])
	{
		if(array[mid] < array[ub])
		{
			pivot = array[mid];
		}
		else if(array[ub] > array[lb])
		{
			pivot = array[ub];
		}
		else
		{
			pivot = array[lb];
		}
	}
	else if(array[mid] > array[ub])
	{
		pivot = array[mid];	
	}
	else if(array[ub] < array[lb])
	{
		pivot = array[ub];
	}
	else
	{
		pivot = array[lb];
	}

	//checks for duplicates
	while(up >= dup_check)
	{
		if(array[dup_check] == pivot)
		{
			dup_check++;
		}
		else if(pivot > array[dup_check])
		{
			temp = array[down];
			array[down++] = array[dup_check];
			array[dup_check++] = temp;
		}
		else
		{
			temp = array[up];
			array[up--] = array[dup_check];
			array[dup_check] = temp;
		}
	}
	*mid_right = ++up;
	return --down;
}
