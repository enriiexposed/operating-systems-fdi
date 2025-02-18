#include <stdio.h>
#include <stdlib.h>

int nelem;

int main(void)
{
	int *ptr;
	int * ptr2;

	nelem = 127;
	ptr = (int*) malloc(nelem*sizeof(int));
	*ptr = 5;
	ptr[0] = 7;
	ptr2 = ptr;

	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);

	ptr[1] = 10;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	ptr2++;
	*ptr2 = 15;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	free(ptr);
	*ptr = 3;
	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);
	return 0;
}

