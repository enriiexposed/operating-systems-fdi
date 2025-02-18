#include <math.h>

double pi = M_PI;

double sinDegrees(double x) {
	double rad = (x*M_PI/180);
	return sin(rad);
}

double cosDegrees(double x) {
	double rad = (x*M_PI/180);
	return cos(rad);
}
