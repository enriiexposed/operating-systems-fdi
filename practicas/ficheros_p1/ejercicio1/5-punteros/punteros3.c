#include <stdio.h>
#include <stdlib.h>

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	ptr = &c;
	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	free(ptr);

}

