#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cassert>

int next_random(int min, int max)
{
	// Just to shorten some of the code
	using namespace std;
	using namespace std::chrono;

	// Marking these as static as I only need to initialize them once,
	// even if I use the function multiple times.
	
	// The random number generator requires a seed, so I'm just using
	// the current time.
	static auto seed = system_clock::now().time_since_epoch().count();
	static default_random_engine generator(seed);

	// Next I'm creating a distribution that will give me a normalized
	// number, that way I can apply whatever random number I get to any
	// range I want!
	static uniform_real_distribution<double> distribution(0.0, 1.0);

	double value = distribution(generator);

	// Here I'm 'denormalizing' the value, so that it fits
	// within the supplied range. I also truncate and convert
	// the value to int.
	return (int)trunc((value * (max - min)) + min);
}

/**
 * This will initialize the matrixes (matri? matrices?) with
 * random numbers.
 */
template <size_t rows, size_t cols>
void generate_matrix(int (&matrix)[rows][cols])
{
	for (size_t row = 0; row < rows; ++row)
		for (size_t col = 0; col < cols; ++col)
			matrix[row][col] = next_random(5, 15);
}

/**
 * This will print the matrix
 */
template <size_t rows, size_t cols>
void print_matrix(int (&matrix)[rows][cols])
{
	for (size_t row = 0; row < rows; ++row)
	{
		printf("[ ");
		for (size_t col = 0; col < cols; ++col)
		{
			if (col != 0)
				printf(", ");
			printf("%d", matrix[row][col]);
		}
		printf(" ]\n");
	}
	printf("\n");
}

/**
 * This will actually perform the dot product, and with templates, we can also
 * make sure that the output matrix is the right size to hold the data we're
 * going to be giving it!
 */
template <size_t rows1, size_t cols1, size_t rows2, size_t cols2>
void dot_product(int (&matrix1)[rows1][cols1], int (&matrix2)[rows2][cols2], int (&outputMatrix)[rows1][cols2])
{
	// Validate matrix dimensions
	assert(cols1 == rows2);

	for (size_t row = 0; row < rows1; ++row)
	{
		for (size_t col = 0; col < cols2; ++col)
		{
			int product = 0;
			for (size_t i = 0; i < cols1; ++i)
				product += matrix1[row][i] * matrix2[i][col];
			outputMatrix[row][col] = product;
		}
	}
}

int main(void)
{
	/** TODO(Jordan):
	 * Find some way to generate these sizes randomly
	 * while still being able to validate them.
	 * (perhaps look into constexpr random numbers?)
	 */
	int matrixA[4][3];
	int matrixB[3][2];
	int matrixC[4][2];

	generate_matrix(matrixA);
	printf("Matrix A:\n");
	print_matrix(matrixA);

	generate_matrix(matrixB);
	printf("Matrix B:\n");
	print_matrix(matrixB);
	
	dot_product(matrixA, matrixB, matrixC);
	printf("Matrix C (AxB):\n");
	print_matrix(matrixC);

	return 0;
}
