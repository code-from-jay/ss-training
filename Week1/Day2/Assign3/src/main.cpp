#include <iostream>

int fib(int n)
{
	return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

int main(int /*argc*/, char** /*argv*/)
{
	std::cout << "Hello, world!\n";
	std::cout << "I'm going to calculate the 900th number in the Fibonacci sequence!\n";

	int value = fib(900);
	std::cout << "The 900th no. in the Fibonacci sequence is " << value << "!\n";

	return 0;
}
