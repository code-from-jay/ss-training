#include <iostream>

/**
 * I intentionally don't define this method
 * because I want the program to crash,
 * doing it like this will allow us to see
 * what type was passed into the method.
 */
template <typename T>
void test(T t);

int main(void)
{
	char c = 0;
	int i = 0;
	float f = 0;
	bool b = true;
	bool b2 = false;

	test(b + b2);
	test(b + c);
	test(b + i);
	test(b + f);

	test(c + b);
	test(c + c);
	test(c + i);
	test(c + f);

	test(i + b);
	test(i + c);
	test(i + i);
	test(i + f);

	test(f + b);
	test(f + c);
	test(f + i);
	test(f + f);

	return 0;
}
