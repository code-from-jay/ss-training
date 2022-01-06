#include <iostream>
#include <cassert>
#include <random> // std::default_random_engine, std::uniform_int_distribution
#include <functional> // std::bind
#include <chrono> // std::chrono::system_clock

typedef std::chrono::system_clock sys_clock;

class Grade
{
public:
	char grade(const int&& score)
	{
		return (4 - (score / 10)) + 'A';
	}
};

int main(void)
{
	// Set up a random number gen that's seeded by the current time
	auto now = sys_clock::now().time_since_epoch().count();
	std::default_random_engine gen;
	gen.seed(now);
	std::uniform_int_distribution<int> dist(10, 40);
	// Basically alias `dist(gen)` to `roll` to make things simple
	auto roll = std::bind(dist, gen);

	int n[4] = { roll(), roll(), roll(), roll() };

	Grade grader;

	assert(grader.grade(15) == 'D');
	assert(grader.grade(28) == 'C');
	assert(grader.grade(33) == 'B');
	assert(grader.grade(40) == 'A');
	for (int i = 0; i < 4; ++i)
	{
		int val = n[i];
		std::cout << "Score = " << val << ": Grade " << grader.grade(std::move(val)) << "\n";
	}

	return 0;
}
