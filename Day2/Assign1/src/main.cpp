#include <iostream>
#include <cassert>

#define assertm(expr, msg) assert(((void)msg, expr))

int main(int /*argc*/, char** /*argv*/)
{
	std::cout << "Select a type to print the size of.\n"
		  << "  1) char\n"
		  << "  2) short\n"
		  << "  3) int\n"
		  << "  4) long\n"
		  << "  5) float\n"
		  << "  6) double\n"
		  << "\n"
		  << "Type to print?: ";

	int response;
	std::cin >> response;

	assertm(std::cin.good(), "User's response was not a number!");
	assertm(response > 0 && response < 7, "User's response was not a valid type!");

	switch (response) {
	case 1:
		std::cout << "The size of char is: " << sizeof(char) << " byte(s)\n";
		break;
	case 2:
		std::cout << "The size of short is: " << sizeof(short) << " byte(s)\n";
		break;
	case 3:
		std::cout << "The size of int is: " << sizeof(int) << " byte(s)\n";
		break;
	case 4:
		std::cout << "The size of long is: " << sizeof(long) << " byte(s)\n";
		break;
	case 5:
		std::cout << "The size of float is: " << sizeof(float) << " byte(s)\n";
		break;
	case 6:
		std::cout << "The size of double is: " << sizeof(double) << " byte(s)\n";
		break;
	default:
		std::cout << "Invalid type.\n";
		break;
	}
	
	return 0;
}
