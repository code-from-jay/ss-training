#ifndef BANKCLI_COMMON_H
#define BANKCLI_COMMON_H

#include <string>

namespace bankcli
{
	std::string prompt(const std::string& prompt, bool protect = false);
	std::string sha256(const std::string& str);
	std::string simplify(std::string str);
}

#endif
