#include <iostream>
#include <cstdio>
#include <cmath>

int main(void) {
	float startingNumber = 1.12345f;
	int iteration = 0;
	std::cout << "This program runs indefinitely, you can stop it with Ctrl-C.\n";
	std::cout << "Press \"Enter\" to continue the interations.\n";
	std::cout << "Iteration " << iteration << ": " << startingNumber;
	while (true) {
		iteration++;
		getchar();
		startingNumber += pow(2, iteration);
		std::cout << "Iteration " << iteration << ": " << startingNumber;
	}
	return 0;
}
