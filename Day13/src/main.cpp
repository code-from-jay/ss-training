#include "shapes.h"

#include <cstdio>

int main(int argc, char** argv)
{
	Triangle triangle{{0, 0}, {5, 5}, {10, 0}};
	printf("Area of Triangle<%d, %d>: %f\n", 5, 10, triangle.area());
	printf("Perimeter of Triangle<%d, %d>: %f\n", 5, 10, triangle.perimeter());
	return 0;
}
