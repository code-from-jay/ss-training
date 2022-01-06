#include <iostream>

struct Test
{
	void myfunc(float f)
	{
		std::cout << "Non-const function called with f{" << f << "}!\n";
	}

	void myfunc(float f) const
	{
		std::cout << "Const function called with f{" << f << "}!\n";
	}
};

int main(void)
{
	const Test a{};
	Test b{};

	a.myfunc(3.14);
	b.myfunc(4.29);
	return 0;
}
