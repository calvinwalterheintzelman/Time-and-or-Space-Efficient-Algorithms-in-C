#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	FILE* fp = fopen("large", "w");
	int i;

	for(i = 0; i < 0; i++)
	{
		fputc(254, fp);
	}

	return EXIT_SUCCESS;
}
