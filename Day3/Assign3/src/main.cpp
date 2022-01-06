#include <vector>
#include <cstdio>

struct Test
{
	int arr[4];
	int extra_value;
};

int main(void)
{
	int big_arr[4] = { 1, 2, 3, 4 };

	std::vector<int> big_vec = { 1, 2, 3, 4 };

	printf("sizeof int[] with 4 elements: %lu bytes\n", sizeof(big_arr));
	printf("sizeof std::vector<int> with 4 elements: %lu bytes\n", sizeof(big_vec) + (big_vec.capacity() * sizeof(int)));

	Test test = { {1, 2, 3, 4 }, 19 };
	printf("Test.extra_value: %d\n", test.extra_value);
	test.arr[4] = 9;
	printf("Test.extra_value: %d\n", test.extra_value);

	return 0;
}
