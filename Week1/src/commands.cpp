#include "commands.h"
#include "account.h"
#include "common.h"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <utility>

#include "rapidfuzz/fuzz.hpp"
#include "rapidfuzz/utils.hpp"

typedef std::chrono::system_clock sc;

std::tm tm =
{
	.tm_sec = 0,
	.tm_min = 0,
	.tm_hour = 0,
	.tm_mday = 12,
	.tm_mon = 11,
	.tm_year = 2021 - 1900,
	.tm_isdst = -1,
};
auto tp = sc::from_time_t(std::mktime(&tm));

std::vector<bankcli::account> bankcli::__internal__::accounts
{
	{ .name = "Jonathan Joestar", .ssn = "104041868", .creation_date = tp, .account_number = 1 },
	{ .name = "Joseph Joestar", .ssn = "109271920", .creation_date = tp, .account_number = 2 },
	{ .name = "Jotaro Kujo", .ssn = "101011970", .creation_date = tp, .account_number = 3 },
	{ .name = "Josuke Higashikata", .ssn = "116231983", .creation_date = tp, .account_number = 4 },
	{ .name = "Giorno Giovanna", .ssn = "104161985", .creation_date = tp, .account_number = 5 },
	{ .name = "Jolyne Cujoh", .ssn = "219581745", .creation_date = tp, .account_number = 6 }
};

using namespace bankcli::__internal__;

static unsigned int counter = 7;

void print_account(const bankcli::account& acc)
{
	const std::time_t t = std::chrono::system_clock::to_time_t(acc.creation_date);
	std::cout << "Customer Name: " << acc.name << "\n"
	          << "SSN: ***-**-" << acc.ssn.substr(5) << "\n"
	          << "Date Opened: " << std::put_time(std::localtime(&t), "%D %I:%M %p %Z\n")
	          << "Account #: " << acc.account_number << "\n";

}

void bankcli::show_accounts()
{
	for (const auto& acc : accounts)
	{
		const std::time_t t = std::chrono::system_clock::to_time_t(acc.creation_date);
		std::cout << "Name: " << acc.name << ", Acc#: " << acc.account_number << ", Date Opened: " << std::put_time(std::localtime(&t), "%D %I:%M %p %Z\n");
	}
}

void bankcli::display_account()
{
	std::string accNumStr = bankcli::prompt("Account> ");
	unsigned int accNum;
	try
	{
		accNum = (unsigned int)std::stoul(accNumStr);
	}
	catch (...)
	{
		std::cout << "Invalid account\n";
		return;
	}

	auto accSearch = std::find_if(
			accounts.begin(),
			accounts.end(),
			[&accNum](const bankcli::account& acc) { return acc.account_number == accNum; });

	if (accSearch != accounts.end())
	{
		const bankcli::account& acc = *accSearch;
		print_account(acc);
	}
	else
	{
		std::cout << "Invalid account\n";
	}
}

void bankcli::search_name()
{
	while (true)
	{
		std::string name = bankcli::prompt("Name> ");
		if (simplify(name) == "home")
			break;

		std::vector<std::pair<bankcli::account, double>> results;

		auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(name);

		for (const auto& acc : accounts)
		{
			double score = scorer.ratio(acc.name, 0.0);
			results.emplace_back(acc, score);
		}

		std::sort(results.begin(), results.end(), [](const auto& r1, const auto& r2)
		{
			return (std::get<double>(r1) > std::get<double>(r2));
		});

		for (int i = 0; i < std::min(3UL, results.size()); ++i)
		{
			auto acc = std::get<bankcli::account>(results[i]);
			std::cout << i + 1 << ") " << acc.name << std::endl;
		}
		int homeNum = std::min(3UL, results.size());
		std::cout << homeNum + 1 << ") Home" << std::endl;

		std::string numStr = bankcli::prompt("Select Number> ");
		if (simplify(numStr) == "home")
			break;
		int num;
		try
		{
			num = std::stol(numStr) - 1;
		}
		catch (...)
		{
			std::cout << "Invalid number" << std::endl;
			continue;
		}

		if (num == homeNum)
			break;

		if (num >= results.size())
		{
			std::cout << "Invalid number" << std::endl;
			continue;
		}

		auto account = std::get<bankcli::account>(results[num]);
		print_account(account);
		break;
	}
}

void bankcli::new_account()
{
	bool validated = false;

	while (!validated)
	{
		std::string customer_name = bankcli::prompt("Customer Name> ");
		std::string ssn = bankcli::prompt("SSN> ");
		
		// Allow for SSN in the format of 123-45-6789, but clean it up
		// so it's stored as 123456789
		ssn.erase(std::remove(ssn.begin(), ssn.end(), '-'), ssn.end());

		if (ssn.length() != 9 || !std::all_of(ssn.begin(), ssn.end(), isdigit))
		{
			std::cout << "Invalid SSN\n";
			continue;
		}

		if (std::find_if(accounts.begin(), accounts.end(), [&ssn](const bankcli::account& acc) { return acc.ssn == ssn; }) != accounts.end())
		{
			std::cout << "Account already exists\n";
			return;
		}

		bankcli::account new_account
		{
			.name = customer_name,
			.ssn = ssn,
			.creation_date = std::chrono::system_clock::now(),
			.account_number = counter++
		};

		accounts.push_back(new_account);
		validated = true;
		std::cout << "Account {" << new_account.account_number << "} created sucessfully!\n";
	}
}

void bankcli::close_account()
{
	std::string numStr = bankcli::prompt("Account #> ");
	if (simplify(numStr) == "home")
		return;

	unsigned int num;
	try
	{
		num = std::stoul(numStr);
	}
	catch (...)
	{
		std::cout << "Invalid account" << std::endl;
		return;
	}

	auto it = std::find_if(accounts.begin(), accounts.end(), [&num](const auto& acc)
	{
		return acc.account_number == num;
	});

	if (it == accounts.end())
	{
		std::cout << "Invalid account" << std::endl;
		return;
	}

	std::string response = simplify(bankcli::prompt("Are you sure you want to close this account? Yes/No> "));
	if (response == "yes" || response == "y")
	{
		accounts.erase(it);
		std::cout << "Account closed" << std::endl;
	}
}

void bankcli::help()
{
	std::cout << "Valid Commands:\n"
		<< "\tshow accounts - List all accounts\n"
		<< "\tdisplay account - Shows info on a specific account\n"
		<< "\tsearch name - Searches for an account by customer name\n"
		<< "\tnew account - Register a new account\n"
		<< "\tclose account - Closes an account\n"
		<< "\thelp - Display this message\n";
}
