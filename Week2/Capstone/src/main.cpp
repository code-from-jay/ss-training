#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "commands.h"
#include "common.h"

#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

// Ideally we'd pull this from a database or server, but for now we'll just embed it into the application.
static const char* administrator_pass = "2a97516c354b68848cdbd8f54a226a0a55b21ed138e207ad6c5cbb9c00aa5aea";

void normalize_string(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
}

#ifdef RUN_TESTS

#define HOLD_STDOUT() \
	std::ostringstream oss; \
	std::streambuf* p_cout_streambuf = std::cout.rdbuf(); \
	std::cout.rdbuf(oss.rdbuf())

#define RELEASE_STDOUT() \
	std::cout.rdbuf(p_cout_streambuf); \
	std::string cout = oss.str()

#define HOLD_STDIN(input) \
	std::istringstream iss{input}; \
	std::streambuf* p_cin_streambuf = std::cin.rdbuf(); \
	std::cin.rdbuf(iss.rdbuf())

#define RELEASE_STDIN() \
	std::cin.rdbuf(p_cin_streambuf); \
	std::string cin = iss.str()

TEST_CASE("checking if sha256 method matches against sha256sum")
{
	CHECK(bankcli::sha256("abcdefg") == "7d1a54127b222502f5b79b5fb0803061152a44f92b37e23c6527baf665d4da9a");
}

TEST_CASE("exposed commands should print and accept valid data")
{
	std::tm tm = {};
	tm.tm_mon = 11;
	tm.tm_mday = 12;
	tm.tm_year = 2021 - 1900;
	auto tp = std::mktime(&tm);

	bankcli::AccountBook account_book;
	{
		bankcli::Account* acc = account_book.add_accounts();
		acc->set_name("Test Case");
		acc->set_ssn("123456789");
		acc->set_creation_date(tp);
		acc->set_id(1);
	}
		
	SUBCASE("`show accounts` prints initial data")
	{
		HOLD_STDOUT();
		bankcli::show_accounts(account_book);
		RELEASE_STDOUT();

		CHECK(cout == "Name: Test Case, Acc#: 1, Date Opened: 12/12/21 12:00 AM UTC\n");
	}

	SUBCASE("`display account` prints valid data")
	{
		HOLD_STDOUT();
		HOLD_STDIN("1\nhome\n");
		bankcli::display_account(account_book);
		RELEASE_STDIN();
		RELEASE_STDOUT();

		CHECK(cout == R"(Customer Name: Test Case
SSN: ***-**-6789
Date Opened: 12/12/21 12:00 AM UTC
Account #: 1
Balance: $0.00

> Add Transaction
> Home
)");
	}

	SUBCASE("`new account` updates the account book with supplied data")
	{
		account_book.clear_accounts();

		HOLD_STDOUT();
		HOLD_STDIN("Test Case\n123456789\n");
		bankcli::new_account(account_book);
		RELEASE_STDIN();
		RELEASE_STDOUT();

		REQUIRE(account_book.accounts_size() == 1);

		const auto& acc = account_book.accounts(0);
		CHECK(acc.name() == "Test Case");
		CHECK(acc.id() == 1);
		CHECK(acc.ssn() == "123456789");
		CHECK(acc.balance() == 0);
		CHECK(acc.transactions_size() == 0);
	}

	SUBCASE("`close account` should remove an account from the account book")
	{
		HOLD_STDOUT();
		HOLD_STDIN("1\ny\n");
		bankcli::close_account(account_book);
		RELEASE_STDIN();
		RELEASE_STDOUT();

		CHECK(account_book.accounts_size() == 0);
	}
}
#else
int main(int argc, char** argv)
{
	bool authenticated = false;

	while (!authenticated)
	{
		/** TODO(Jordan):
		 * Put this in a little ncurses window to look prettier.
		 */
		std::string username = bankcli::prompt("User: ");
		std::string password = bankcli::prompt("Password: ", true);

		if (username == "demo" && bankcli::sha256(password) == administrator_pass)
			authenticated = true;
		else
			printf("Incorrect user.\n");
	}

	// Load account book from a file if it exists
	bankcli::AccountBook account_book;
	{
		std::fstream input("accounts.bank", std::ios::in | std::ios::binary);
		if (input)
		{
			assert(account_book.ParseFromIstream(&input));
		}
	}

	while (true)
	{
		/** TODO(Jordan):
		 * Look for some way to 'suggest' commands,
		 * like, if a user starts to type "show", then
		 * perhaps have a slightly dim " accounts" on the
		 * prompt.
		 */
		std::string command = bankcli::prompt("Home> ");

		// Somewhat normalize the command
		normalize_string(command);

		if (command == "quit")
			break;
		else if (command == "show accounts")
			bankcli::show_accounts(account_book);
		else if (command == "show transactions")
			bankcli::show_transactions(account_book);
		else if (command == "display account")
			bankcli::display_account(account_book);
		else if (command == "search name")
			bankcli::search_name(account_book);
		else if (command == "new account")
			bankcli::new_account(account_book);
		else if (command == "close account")
			bankcli::close_account(account_book);
		else if (command == "help")
			bankcli::help();
		else
			printf("Invalid command.\n");
	}

	// Write any changes to accounts.bank
	{
		std::fstream output("accounts.bank", std::ios::out | std::ios::trunc | std::ios::binary);
		assert(account_book.SerializeToOstream(&output));
	}

	google::protobuf::ShutdownProtobufLibrary();
	
	return 0;
}
#endif
