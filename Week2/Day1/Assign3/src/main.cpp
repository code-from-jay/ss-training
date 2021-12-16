#include <iostream>

bool is_power_of_2(int val)
{
	if (val == 2) return true;
	else if (val < 2) return false;

	// Making this a long just to make sure that if val is MAX_INT the 
	// code won't run forever.
	long powered = 2;
	while (powered < val)
		powered += powered;

	return powered == val;
}

int main(int argc, char** argv)
{
	int val;
	std::cout << "Enter a value to check: ";
	std::cin >> val;
	std::cout << val << " " << (is_power_of_2(val) ? "is" : "is not") << " a power of 2!\n";
	return 0;
}
