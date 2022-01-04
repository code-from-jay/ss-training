#ifndef BANKCLI_COMMANDS_H
#define BANKCLI_COMMANDS_H

#include "account.pb.h"

namespace bankcli
{
	void show_accounts(const bankcli::AccountBook& account_book);
	void show_transactions(const bankcli::AccountBook& account_book);
	void display_account(bankcli::AccountBook& account_book);
	void search_name(const bankcli::AccountBook& account_book);
	void new_account(bankcli::AccountBook& account_book);
	void close_account(bankcli::AccountBook& account_book);
	void help();
}

#endif
