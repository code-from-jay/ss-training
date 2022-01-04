#include "common.h"
#include "Sha256.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include <termios.h>
#include <unistd.h>

#ifndef RUN_TESTS
int getch(bool protect)
{
	if (!protect)
		return getchar();

	int ch;
	struct termios t_old, t_new;

	tcgetattr(STDIN_FILENO, &t_old);
	t_new = t_old;
	t_new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
	return ch;
}

std::string bankcli::prompt(const std::string& prompt, bool protect)
{
	constexpr char BACKSPACE = 127;
	constexpr char RETURN = 10;

	std::cout << prompt;
	std::string response;

	unsigned char ch = 0;
	while ((ch = getch(protect)) != RETURN)
	{
		if (ch == BACKSPACE)
		{
			if (response.length() != 0)
			{
				std::cout << "\b \b";
				response.resize(response.length()-1);
			}
		}
		else
		{
			response += ch;
			if (protect)
				std::cout << '*';
		}
	}

	if (protect)
		std::cout << std::endl;
	return response;
}
#else
std::string bankcli::prompt(const std::string&, bool protect)
{
	char buff[256];
	std::cin.getline(buff, 256);
	return std::string{buff};
}
#endif

std::string bankcli::sha256(const std::string& str)
{
	SHA256_HASH digest;
	Sha256Calculate(str.c_str(), str.size(), &digest);

	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < SHA256_HASH_SIZE; ++i)
	{
		ss << std::hex << std::setw(2) << static_cast<int>(digest.bytes[i]);
	}
	return ss.str();
}

std::string bankcli::simplify(std::string str)
{
	// Make string lowercase
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
	{
		return std::tolower(c);
	});

	char chars[] = ".,()|-";

	for (const auto& c : chars)
	{
		str.erase(std::remove(str.begin(), str.end(), c), str.end());
	}

	return str;
}
