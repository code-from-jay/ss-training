#include <cstdio>

void fib_array(int* array, size_t size, size_t iteration = 0)
{
	if (iteration == size)
		return;

	if (iteration == 0)
		array[0] = 0;
	else if (iteration == 1)
		array[1] = 1;
	else
		array[iteration] = array[iteration-1] + array[iteration-2];
	fib_array(array, size, iteration + 1);
}

int main(void)
{
	int arr[50] = {};
	fib_array(arr, 50);

	printf("[ %d", arr[0]);
	for (int i = 1; i < 50; ++i)
		printf(", %d", arr[i]);
	printf(" ]\n");

	return 0;
}
