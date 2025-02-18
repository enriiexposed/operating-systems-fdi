#include <stdio.h>


#define ARRAY_SIZE  10

int a = 7;
unsigned long b = 8;
short c;
char x;
char* pc;

int array1[ARRAY_SIZE];
int array2[a];

char* str1    = "STRING OF CHARACTERS";
char str2[]   = "STRING OF CHARACTERS";
int main()
{
	pc = &x;
	a = 16;
	printf("Address of a: %p Size: %lu \n",&a,sizeof(a));
	printf("Address of b: %p Size: %lu \n",&b,sizeof(b));
	printf("Address of c: %p Size: %lu \n",&c,sizeof(c));
	printf("Address of x: %p Size: %lu \n",&x,sizeof(x));
	printf("Address of pc: %p Address pointed by pc: %p Size: %lu \n",
			&pc, pc, sizeof(pc));
	printf("Address of array: %p Addres of elemnt 0: %p Size of the array: %lu \n",
			array1, &array1[0], sizeof(array1));
	printf("Address of str1: %p Address pointed by it: %p Size: %lu \n",
			&str1, str1, sizeof(str1));
	printf("Address of str2: %p Address pointed by it: %p Size: %lu \n",
			&str2, str2, sizeof(str2));
    return 0;
}
