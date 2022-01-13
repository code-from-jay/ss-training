#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <exception>
#include <algorithm>
#include <bitset>

class InvalidIntStringFormat : public std::exception
{
	std::string _what;
public:
	InvalidIntStringFormat(const std::string& str)
	{
		std::stringstream ss;
		ss << "The string '" << str << "' is not in a valid format for conversion!";
		_what = ss.str();
	}

	const char* what() const noexcept override
	{
		return _what.c_str();
	}
};

std::vector<std::string> prompt(const char* prompt)
{
	std::cout << prompt << " ";
	std::vector<std::string> elements;
	std::string line;
	std::getline(std::cin, line);
	std::istringstream iss(line);
	for (std::string element; std::getline(iss, element, ' ');)
	{
		std::transform(element.begin(), element.end(), element.begin(), [](unsigned char c) { return std::tolower(c); });
		elements.push_back(element);
	}
	return elements;
}

long parse_int(const std::string& src) try
{
	if (src.size() > 2 && src[0] == '0')
	{
		if (src[1] == 'x')
		{
			return std::stol(src, nullptr, 16);
		}
		else if (src[1] == 'b')
		{
			std::string clean{src.c_str()+2};
			return std::stol(clean, nullptr, 2);
		}
	}

	return std::stol(src);
}
catch (const std::invalid_argument&)
{
	throw InvalidIntStringFormat(src);
}
catch (const std::out_of_range&)
{
	throw InvalidIntStringFormat(src);
}

int main()
{
	while (true)
	{
		try
		{
			auto elements = prompt("prompt>");
			if (elements.size() == 0) continue;
			else if (elements.size() == 1) // quit or print
			{
				if (elements[0] == "quit" || elements[0] == "exit")
					break;

				long value = parse_int(elements[0]);
				std::cout << std::setw(20) << value << std::endl;
				continue;
			}
			else if (elements.size() == 2) // 'function' call
			{
				if (elements[0] == "hex")
				{
					long value = parse_int(elements[1]);
					std::cout << std::setw(20) << "0x" << std::hex << value << std::endl;
					continue;
				}
				else if (elements[0] == "bin")
				{
					long value = parse_int(elements[1]);
					std::bitset<64> binval(value);
					bool print = false;
					std::stringstream output;
					output << "0b";
					for (int i = 63; i >= 0; --i)
					{
						if (!print && binval[i])
						{
							print = true;
						}

						if (print)
							output << binval[i];
					}
					std::cout << std::setw(20) << output.str() << std::endl;
					continue;
				}
			}
			else if (elements.size() == 3) // Addition
			{
				long lval = parse_int(elements[0]);
				long rval = parse_int(elements[2]);

				long nval = lval + rval;
			
				std::cout << std::setw(20) << nval << std::endl;
				continue;
			}

			std::cout << "Invalid expression!" << std::endl;
		}
		catch (const InvalidIntStringFormat& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}
