#include "auxiliar.h"
#include <stdio.h>

extern double pi;

int main()
{
	int degrees;
	double rad;

	printf("Introduce an angle expressed in degrees: ");
	scanf("%d",&degrees);
	printf("\n");

	rad = (degrees*pi)/180;
	printf("%d degress are equivalent to %f radians\n", degrees, rad);

	printf("sin(%d) = %f. cos(%d) = %f\n",degrees, sinDegrees(degrees), degrees, cosDegrees(degrees));	
	return 0;
}
