#include <cmath>
#include <cstdio>
#include <cassert>
#include <string_view>
#include <string>
#include <iostream>
#include <algorithm>

int count_digits(const std::string_view& str)
{
	int count = 0;
	bool decimal = false;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == '.')
		{
			decimal = true;
		}
		else if (decimal)
		{
			if (!std::isdigit(str[i]))
				break;
			++count;

		}
	}

	return count;
}

int main(int argc, char** argv)
{
	std::string decimal;
	std::cin >> decimal;
	try
	{
		std::stod(decimal);
	}
	catch (...)
	{
		std::cout << "Not a valid decimal!\n";
		return 0;
	}

	int den = pow(10, count_digits(decimal)-1);
	assert(den != 0);

	std::string numstr = decimal;
	numstr.erase(std::remove(numstr.begin(), numstr.end(), '.'), numstr.end());

	int num = std::stoi(numstr);
	assert(num != 0);

	printf("%d/%d\n", num, den);

	/**
	 * Still not all that sure on how this all works
	 * as from my understand we're trying to get an N
	 * so that J is >= 2**52 and < 2**53
	 */
	long long J = 0;
	int N = 1;
	while (true)
	{
		long long temp = (round(pow(2, N++)*((double)num/den)));
		if (temp >= pow(2, 52) && temp < pow(2, 53))
		{
			J = temp;
			break;
		}
	}

	printf("J: %lld, N: %d\n%.40f\n", J, N, (double)J/pow(2, N));

	return 0;
}
