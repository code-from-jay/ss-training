#include <vector>
#include <iostream>

#define ARR_SIZE 3

int main(int argc, char** argv)
{
	// Initialize the array of floats with simple values
	float arr[ARR_SIZE] = { 1.57f, 3.14f, 6.28f };

	// Append each value of arr to the end of the vector.
	std::vector<float> vec;
	vec.insert(vec.end(), &arr[0] /* start of arr */, &arr[ARR_SIZE] /* end of arr */);

	// Just to keep formatting consistent
	bool first = true;

	// C++'s version of foreach, thank you iterators!
	for (float& f : vec)
	{
		if (first)
			first = false;
		else
			std::cout << " ";

		std::cout << f;
	}

	// And to cap it all off, EOL
	std::cout << "\n";

	return 0;
}
