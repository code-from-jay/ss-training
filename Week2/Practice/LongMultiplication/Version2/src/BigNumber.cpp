#include "BigNumber.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cassert>

typedef std::vector<unsigned char> charvec;

BigNumber::BigNumber(long input)
{
	this->set(input);
}

BigNumber::BigNumber(const std::string& input)
{
	this->set(input);
}

void BigNumber::set(long input)
{
	this->_number = input;
	this->_overflown = false;
}

void BigNumber::set(const std::string& input)
{
	assert(input.size() > 0);
	assert(input.size()+1 <= 64);
	try
	{
		this->_number = std::stol(input);
	}
	catch (const std::out_of_range&)
	{
		for (int i = 0; i < input.size(); ++i)
		{
			this->_numstr[i] = input[i];
		}
		this->_numstr[input.size()] = '\0';
		this->_overflown = true;
	}
}

std::string BigNumber::number() const
{
	return this->_overflown ? std::string((char*)this->_numstr) : std::to_string(this->_number);
}

std::string add_strings(std::string A, std::string B)
{
	bool negative = false;
	if (A[0] == '-')
	{
		negative = !negative;
		A.erase(0, 1);
	}
	if (B[0] == '-')
	{
		negative = !negative;
		B.erase(0, 1);
	}

	if (A.size() < B.size())
		A = std::string(B.size() - A.size(), '0');
	else if (B.size() < A.size())
		B = std::string(A.size() - B.size(), '0');

	std::string sum = "";

	int carry = 0;
	for (int i = A.size()-1; i >= 0; --i)
	{
		int result = (A[i] - '0') + (B[i] - '0') + carry;
		if (result >= 10)
		{
			carry = 1;
			result -= 10;
		}
		else
		{
			carry = 0;
		}

		assert(result < 10);

		sum += ((char)result) + '0';
	}

	if (carry > 0)
		sum += ((char)carry) + '0';

	if (negative)
		sum += '-';

	std::reverse(sum.begin(), sum.end());
	return sum;
}

numvec BigNumber::simulate_multiply(const BigNumber& input) const
{
	auto digits_in = input.number();
	auto digits_self = this->number();
	numvec numbers;
	numbers.emplace_back(charvec{digits_self.begin(), digits_self.end()});
	numbers.emplace_back(charvec{digits_in.begin(), digits_in.end()});

	bool negative = digits_in[0] == '-';

	if (this->_overflown || input._overflown)
	{
		bool self_negative = digits_self[0] == '-';
		std::string bigsum = "";
		for (int in_pos = digits_in.size()-1; in_pos >= 0; --in_pos)
		{
			if (in_pos == 0 && negative)
				break;

			int in_places = (digits_in.size()-1) - in_pos;
			std::string small_sum = "";

			for (int self_pos = digits_self.size()-1; self_pos >= 0; --self_pos)
			{
				if (self_pos == 0 && self_negative)
					break;

				int self_places = (digits_self.size()-1) - self_pos;
				int result = (digits_self[self_pos] - '0') * (digits_in[in_pos] - '0');
				if (self_negative ^ negative)
					result *= -1;
				if (small_sum == "")
				{
					small_sum = std::to_string(result);
					if (self_places > 0 || in_places > 0)
						small_sum += std::string(self_places + in_places, '0');
				}
				else
				{
					std::string result_str = std::to_string(result);
					if (self_places > 0 || in_places > 0)
						result_str += std::string(self_places + in_places, '0');
					small_sum = add_strings(result_str, small_sum);
				}
			}

			numbers.emplace_back(charvec{small_sum.begin(), small_sum.end()});
			bigsum = add_strings(small_sum, bigsum);
		}

		numbers.emplace_back(charvec{bigsum.begin(), bigsum.end()});
	}
	else
	{
		long sum = 0;
		for (int pos = digits_in.size() - 1; pos >= 0; --pos)
		{
			if (pos == 0 && negative)
				break;

			long mult = digits_in[pos] - '0';
			mult *= (long)pow(10, (digits_in.size()-1) - pos);
			if (negative)
				mult *= -1;
			long result = this->_number * mult;
			sum += result;
			std::string resultstr = std::to_string(result);
			numbers.emplace_back(charvec{resultstr.begin(), resultstr.end()});
		}
		std::string sumstr = std::to_string(sum);
		numbers.emplace_back(charvec{sumstr.begin(), sumstr.end()});
	}

	return numbers;
}

BigNumber BigNumber::multiply(const BigNumber& input) const
{
	auto vec = simulate_multiply(input);
	auto last = vec.back();
	return BigNumber{std::string(last.begin(), last.end())};
}

void BigNumber::print(const numvec& myvec) const
{
	// myvec should contains the 2 values to multiply
	// at least one 'step', and finally the result
	// meaning at minimum myvec should be 4 elements
	assert(myvec.size() >= 4);

	int longest = (*std::max_element(myvec.begin()+2, myvec.end(), [](const auto& A, const auto& B)
	{
		return A.size() < B.size();
	})).size()+1;

	std::cout << std::setw(longest) << std::right << vtos(myvec[0]) << "\n";
	std::cout << std::setw(longest) << std::right << std::string("x ") + vtos(myvec[1]) << "\n";
	std::cout << std::setw(longest) << std::right << std::string(myvec[1].size() + 2, '-') << "\n";

	for (int i = 2; i < myvec.size()-1; ++i)
		std::cout << std::setw(longest) << std::right << vtos(myvec[i]) << "\n";

	std::cout << std::string(longest, '-') << "\n";
	std::cout << std::setw(longest) << std::right << vtos(myvec[myvec.size()-1]) << "\n\n";
}
