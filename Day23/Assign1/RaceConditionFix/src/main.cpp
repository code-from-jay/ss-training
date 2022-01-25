#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <cassert>

std::mutex processing_mutex;

void process_hailstone(std::vector<int>& hailstone)
{
	while (hailstone.size() < 10000)
	{
		processing_mutex.lock();
		int n = hailstone[hailstone.size()-1];
		hailstone.push_back(n % 2 == 0 ? n/2 : 3*n+1);
		processing_mutex.unlock();
	}
}

int main(int argc, char** argv)
{
	std::vector<int> hailstone;
	hailstone.reserve(10000);
	hailstone.push_back(5);
	std::thread t1(process_hailstone, std::ref(hailstone));
	std::thread t2(process_hailstone, std::ref(hailstone));
	std::thread t3(process_hailstone, std::ref(hailstone));

	t1.join();
	t2.join();
	t3.join();

	assert(hailstone.size() != 0);

	bool violation = false;
	for (int i = 1; i < hailstone.size(); ++i)
	{
		int previous = hailstone[i-1];
		int expected_value = previous % 2 == 0 ? previous/2 : 3*previous+1;
		if (expected_value != hailstone[i])
		{
			std::cout << "Variants violated in hailstone!\n";
			violation = true;
			break;
		}
	}

	if (!violation)
		std::cout << "Hailstone processed with no variants violated!\n";

	std::cout << "[ ";
	for (int i = 0; i < 100; ++i)
	{
		if (i != 0)
			std::cout << ", ";
		std::cout << hailstone[i];
	}
	std::cout << ", ...  ]\n";

	return 0;
}
