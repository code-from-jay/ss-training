#ifndef BANKCLI_COMMANDS_H
#define BANKCLI_COMMANDS_H

#include <vector>

namespace bankcli
{
	void show_accounts();
	void display_account();
	void search_name();
	void new_account();
	void close_account();
	void help();

	/**
	 * This is only here so I can expose functionality to tests
	 * and wouldn't be around when a database or equivalent
	 * were used.
	 */
	struct account;

	namespace __internal__
	{
		extern std::vector<bankcli::account> accounts;
	}
}

#endif
