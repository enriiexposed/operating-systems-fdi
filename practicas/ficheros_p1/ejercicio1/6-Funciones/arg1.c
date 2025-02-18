#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
int sum(int a, int b);
struct _complex_  sumC( struct _complex_ a,  struct _complex_ b);

int main(void)
{
	int x = 4, y = 5;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	struct _complex_ zc;

	zc = sumC(xc, yc);

	int total = sum(x, y);

	printf("Complex numbers addition. (%f,%f i) + (%f,%f i) =(%f,%f i)\n",xc.re,xc.im,yc.re,yc.im,zc.re,zc.im);
	printf("Integer addition:  x +y = %d + %d = %d \n",x,y, total);
	return 0;
}

int sum(int x, int y)
{
	int c;
	c = x +y;
	x = 7;
	y =3;
	return c;
}


struct _complex_  sumC( struct _complex_ a,  struct _complex_ b)
{
	struct _complex_ r;
	r.re = a.re + b.re;
	r.im = a.im + b.im;

	// We modify the first argument
	a.re = 12.5;
	a.im = 13.4;
	return r;
}
