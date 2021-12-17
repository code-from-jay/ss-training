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
	switch (funcmask)
	{
		case ADD_OP:
			return operation_functions[0](arg1, arg2);
		case SUB_OP:
			return operation_functions[1](arg1, arg2);
		case MUL_OP:
			return operation_functions[2](arg1, arg2);
		default:
			return 0;
	}
}

int main(int argc, char** argv)
{
	assert(compute(9, 10, ADD_OP) == 19);
	assert(compute(4, 10, SUB_OP) == -6);
	assert(compute(5, 2, MUL_OP) == 10);
	return 0;
}
