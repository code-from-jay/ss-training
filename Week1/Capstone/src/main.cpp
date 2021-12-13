#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>

#include "account.h"
#include "commands.h"
#include "common.h"

#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

// Ideally we'd pull this from a database or server, but for now we'll just embed it into the application.
static const char* administrator_pass = "11c32bbe6d998b554bf8be682d9b0a56a1d122003f7804b76b16c2dd82613789";

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
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		
	SUBCASE("`show accounts` prints initial data")
	{
		bankcli::__internal__::accounts =
		{
			{ .name = "Test Case", .ssn = "123456789", .creation_date = tp, .account_number = 1 }
		};

		HOLD_STDOUT();
		bankcli::show_accounts();
		RELEASE_STDOUT();

		CHECK(cout == "Name: Test Case, Acc#: 1, Date Opened: 12/12/21 12:00 AM UTC\n");
	}

	SUBCASE("`display account` prints valid data")
	{
		bankcli::__internal__::accounts =
		{
			{ .name = "Test Case", .ssn = "123456789", .creation_date = tp, .account_number = 1 }
		};

		HOLD_STDOUT();
		HOLD_STDIN("1\n");
		bankcli::display_account();
		RELEASE_STDIN();
		RELEASE_STDOUT();

		CHECK(cout == R"(Customer Name: Test Case
SSN: ***-**-6789
Date Opened: 12/12/21 12:00 AM UTC
Account #: 1
)");
	}

	SUBCASE("`new account` updates the accounts vector with supplied data")
	{
		bankcli::__internal__::accounts.clear();

		HOLD_STDOUT();
		HOLD_STDIN("Test Case\n123456789\n");
		bankcli::new_account();
		RELEASE_STDIN();
		RELEASE_STDOUT();

		REQUIRE(bankcli::__internal__::accounts.size() == 1);

		const auto& acc = bankcli::__internal__::accounts[0];
		CHECK(acc.name == "Test Case");
		CHECK(acc.account_number == 7);
		CHECK(acc.ssn == "123456789");
	}

	SUBCASE("`close account` should remove an account entry from the accounts vector")
	{
		bankcli::__internal__::accounts =
		{
			{ .name = "Test Case", .ssn = "123456789", .creation_date = tp, .account_number = 1 }
		};

		HOLD_STDOUT();
		HOLD_STDIN("1\ny\n");
		bankcli::close_account();
		RELEASE_STDIN();
		RELEASE_STDOUT();

		CHECK(bankcli::__internal__::accounts.size() == 0);
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

		if (username == "Admin" && bankcli::sha256(password) == administrator_pass)
			authenticated = true;
		else
			printf("Incorrect user.\n");
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
			bankcli::show_accounts();
		else if (command == "display account")
			bankcli::display_account();
		else if (command == "search name")
			bankcli::search_name();
		else if (command == "new account")
			bankcli::new_account();
		else if (command == "close account")
			bankcli::close_account();
		else
			printf("Invalid command.\n");
	}
	
	return 0;
}
#endif
