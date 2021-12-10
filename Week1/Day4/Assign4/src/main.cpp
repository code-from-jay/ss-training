#include <cassert>

int main(void)
{
	int x = 4;
	/**
	 * This is essentially equivalent to
	 * `int y = (x++) + 2;`
	 * and since the post-increment operator
	 * is used, x will only be incremented after the expression is finished!
	 *
	 * So it's actually more akin to:
	 * int y = x + 2;
	 * x = x + 1;
	 */
	int y = x+++2;

	assert(y == 6);
	assert(x == 5);
}
