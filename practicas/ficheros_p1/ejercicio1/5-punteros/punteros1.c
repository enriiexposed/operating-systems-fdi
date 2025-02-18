#include <stdio.h>
#include <stdlib.h>

int c = 7;
int main(void)
{
	int *ptr;
	printf("Address of ptr %p. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	ptr = &c;
	printf("Address of ptr %p,. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	*ptr = 4;
	printf("ptr points to %p. Content in the address pointed by ptr: %d Address of c: %p Value of c %d\n",
			ptr, *ptr, &c, c);

	ptr =  (int*) 0x600a48;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

	*ptr = 13;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

    return 0;
}

