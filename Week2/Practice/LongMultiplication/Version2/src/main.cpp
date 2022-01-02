#include "BigNumber.h"

#include <cassert>
#include <limits>
#include <iostream>

long prompt_and_validate(const char* prompt)
{
	long val{};
	while (true)
	{
		std::cout << prompt;
		std::cin >> val;
		// If a long could not be parsed from stdin
		if (std::cin.fail())
		{
			// Clear whatever is currently in std::cin
			std::cin.clear();
			// Then skip whatever else is on this line
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			return val;
		}
	}
}

int main(int argc, char** argv)
{
	long Aval = prompt_and_validate("First number: ");
	long Bval = prompt_and_validate("Second number: ");
	BigNumber A{Aval}, B{Bval};

	auto simulation = A.simulate_multiply(B);
	A.print(simulation);
	
	return 0;
}
