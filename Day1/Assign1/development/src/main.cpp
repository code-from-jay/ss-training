#include <cstdio>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

int main(int /*argc*/, char** /*argv*/)
{
	printf("Hello, world!\n");
	assertm(2 + 2 == 5, "Checking if asserts are working");

	return 0;
}
