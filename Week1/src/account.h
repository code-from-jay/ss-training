#ifndef BANKCLI_ACCOUNT_H
#define BANKCLI_ACCOUNT_H

#include <string>
#include <chrono>

namespace bankcli
{
	typedef std::chrono::time_point<std::chrono::system_clock> timestamp;

	struct account
	{
		std::string name;
		std::string ssn;
		timestamp creation_date;
		unsigned int account_number;
	};
}

#endif
