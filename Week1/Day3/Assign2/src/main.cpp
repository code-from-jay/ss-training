#include <cstdio>

/**
 * To make a dissimilar type, I'm using a union.
 * This will allow me to freely add integers or floats
 * to my array.
 *
 * The catch here is that there's *no way* for me to
 * reliably automatically determine whether the value
 * is an integer or a float without using some
 * external class or a struct.
 */

typedef union
{
	float f;
	int i;
} mixed_types;

int main(int argc, char** argv)
{
	mixed_types arr[4] = { { .i = 20 }, { .f = 2.12f },  { .i = 9 }, { .f = 3.14f } };
	
	printf("Array: [ Int: %d, Float: %f, Int: %d, Float: %f ]\n", arr[0].i, arr[1].f, arr[2].i, arr[3].f);

	for (int i = 0; i < 4; ++i)
	{
		printf("Element %d: { i: %d, f: %f }\n", i, arr[i].i, arr[i].f);
	}
	return 0;
}
