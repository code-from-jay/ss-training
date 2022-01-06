#include <cassert>

typedef int (*operation)(int, int);

operation operation_functions[3] =
{
	[](int arg1, int arg2) -> int { return arg1 + arg2; },
	[](int arg1, int arg2) -> int { return arg1 - arg2; },
	[](int arg1, int arg2) -> int { return arg1 * arg2; }
};

#define ADD_OP 0b001
#define SUB_OP 0b010
#define MUL_OP 0b100

int compute(int arg1, int arg2, unsigned char funcmask)
{
	return operation_functions[funcmask >> 1](arg1, arg2);
}

int main(void)
{
	assert(compute(9, 10, ADD_OP) == 19);
	assert(compute(4, 10, SUB_OP) == -6);
	assert(compute(5, 2, MUL_OP) == 10);
	return 0;
}
