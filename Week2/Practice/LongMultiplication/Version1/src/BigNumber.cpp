#include "BigNumber.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cassert>

BigNumber::BigNumber(long input)
{
	this->set(input);
}

void BigNumber::set(long input)
{
	this->_number = input;
}

long BigNumber::number() const
{
	return this->_number;
}

void BigNumber::multiply(const BigNumber& input, BigNumber& output) const
{
	long input_num = input.number();
	int length = snprintf(NULL, 0, "%ld", input_num) + 1;
	assert(length > 1);
	char* digits = new char[length];
	snprintf(digits, length, "%ld", input_num);

	std::vector<std::string> numbers;
	numbers.emplace_back(std::to_string(this->_number));
	numbers.emplace_back(std::to_string(input_num));

	bool negative = digits[0] == '-';
	long sum = 0;

	for (int pos = length-2; pos >= 0; --pos)
	{
		if (pos == 0 && negative)
			break;

		long mult = digits[pos] - '0';
		mult *= (long)pow(10, (length-2) - pos);
		if (negative)
			mult *= -1;
		long result = this->_number * mult;
		numbers.emplace_back(std::to_string(result));
		sum += result;
	}

	int longest = (*std::max_element(numbers.begin()+2, numbers.end(), [](const auto& A, const auto& B)
	{
		return A.size() < B.size();
	})).size()+1;
	std::cout << std::setw(longest) << std::right << numbers[0] << "\n";
	std::cout << std::setw(longest) << std::right << std::string("x ") + numbers[1] << "\n";
	std::cout << std::setw(longest) << std::right << std::string(numbers[1].size() + 2, '-') << "\n";

	for (int i = 2; i < numbers.size(); ++i)
	{
		std::cout << std::setw(longest) << std::right << numbers[i] << "\n";
	}

	std::cout << std::string(longest, '-') << "\n";
	std::cout << std::setw(longest) << std::right << sum << "\n\n";

	delete[] digits;
	output.set(sum);
}
