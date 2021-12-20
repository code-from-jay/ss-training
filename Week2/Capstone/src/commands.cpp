#include "commands.h"
#include "common.h"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <ctime>
#include <cmath>

#include "rapidfuzz/fuzz.hpp"
#include "rapidfuzz/utils.hpp"

typedef std::chrono::system_clock sc;

/*std::tm tm =
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

using namespace bankcli::__internal__;*/

static unsigned int counter = 1;

uint64_t encode_balance(std::string balance)
{
	// Remove a dollar sign if one is included
	balance.erase(std::remove(balance.begin(), balance.end(), '$'), balance.end());

	size_t pos = balance.find(".");
	assert(pos != std::string::npos);

	std::string dollarstr = balance.substr(0, pos);
	balance.erase(0, pos + 1);

	std::string centstr = balance;

	uint64_t dollars = std::stoi(dollarstr);
	uint64_t cents = std::stoi(centstr);
	return (dollars * (uint64_t)pow(10, 4)) + (cents * (uint64_t)pow(10, 2));
}

std::string decode_balance(uint64_t balance)
{
	int dollars = balance / (int)pow(10, 4);
	int cents = (balance % (int)pow(10, 4)) / (int)pow(10, 2);
	int remainder = (balance % (int)pow(10, 4)) % (int)pow(10, 2);
	if (remainder > 50)
		cents++;
	while (cents > 99)
	{
		++dollars;
		cents -= 100;
	}

	std::stringstream ss;
	ss << "$" << dollars << "." << std::setw(2) << std::setfill('0') << cents;
	return ss.str();
}

void print_account(const bankcli::Account& acc)
{
	std::time_t time = (std::time_t)acc.creation_date();
	std::cout << "Customer Name: " << acc.name() << "\n"
	          << "SSN: ***-**-" << acc.ssn().substr(5) << "\n"
	          << "Date Opened: " << std::put_time(std::localtime(&time), "%D %I:%M %p %Z\n")
	          << "Account #: " << acc.id() << "\n"
		  << "Balance: " << decode_balance(acc.balance()) << "\n";

}

void bankcli::show_accounts(const bankcli::AccountBook& account_book)
{
	for (const auto& acc : account_book.accounts())
	{
		std::time_t time = (std::time_t)acc.creation_date();
		std::cout << "Name: " << acc.name() << ", Acc#: " << acc.id() << ", Date Opened: " << std::put_time(std::localtime(&time), "%D %I:%M %p %Z\n");
	}
}

void bankcli::show_transactions(const bankcli::AccountBook& account_book)
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

	const auto& accounts = account_book.accounts();
	auto accSearch = std::find_if(
			accounts.begin(),
			accounts.end(),
			[&accNum](const bankcli::Account& acc) { return acc.id() == accNum; });

	if (accSearch != accounts.end())
	{
		const bankcli::Account& acc = *accSearch;
		std::cout << "Transaction History for " << acc.name() << "\n";
		std::cout << "Current Balance: " << decode_balance(acc.balance()) << "\n";
		std::cout << "------------------------" << std::setw(acc.name().size()) << std::setfill('-') << '-';
		std::cout << "\n";
		for (const auto& transaction : acc.transactions())
		{
			bool credit = transaction.type() == bankcli::Account::CREDIT;
			std::cout
				<< (credit ? "Credit: +" : "Debit:  -")
				<< decode_balance(transaction.amount())
				<< "\n";
		}
	}
	else
	{
		std::cout << "Invalid account\n";
	}
}

void add_transaction(bankcli::Account& acc)
{
	std::string typestr = bankcli::simplify(bankcli::prompt("Debit/Credit?> "));
	bankcli::Account::TransactionType type;
	if (typestr == "debit")
	{
		type = bankcli::Account::DEBIT;
	}
	else if (typestr == "credit")
	{
		type = bankcli::Account::CREDIT;
	}
	else
	{
		std::cout << "Invalid transaction type.\n";
		return;
	}

	std::string amountStr = bankcli::prompt("Amount> ");
	int amount = encode_balance(amountStr);

	if (type == bankcli::Account::DEBIT && amount > acc.balance())
	{
		std::cout << "Insufficient funds.\n";
		return;
	}

	if (type == bankcli::Account::DEBIT)
	{
		acc.set_balance(acc.balance() - amount);
	}
	else
	{
		acc.set_balance(acc.balance() + amount);
	}

	bankcli::Account_Transaction* transaction = acc.add_transactions();
	transaction->set_type(type);
	transaction->set_amount(amount);
}

void bankcli::display_account(bankcli::AccountBook& account_book)
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

	auto accounts = account_book.mutable_accounts();
	auto accSearch = std::find_if(
			accounts->begin(),
			accounts->end(),
			[&accNum](const bankcli::Account& acc) { return acc.id() == accNum; });

	if (accSearch != accounts->end())
	{
		bankcli::Account& acc = *accSearch;
		print_account(acc);

		std::cout << "\n> Add Transaction\n> Home\n";
		std::string command = simplify(bankcli::prompt("Command?> "));
		if (command == "add transaction")
			add_transaction(acc);
	}
	else
	{
		std::cout << "Invalid account\n";
	}
}

void bankcli::search_name(const bankcli::AccountBook& account_book)
{
	while (true)
	{
		std::string name = bankcli::prompt("Name> ");
		if (simplify(name) == "home")
			break;

		std::vector<std::pair<const bankcli::Account*, double>> results;

		auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(name);

		for (const auto& acc : account_book.accounts())
		{
			double score = scorer.ratio(acc.name(), 0.0);
			results.emplace_back(&acc, score);
		}

		std::sort(results.begin(), results.end(), [](const auto& r1, const auto& r2)
		{
			return (std::get<double>(r1) > std::get<double>(r2));
		});

		for (int i = 0; i < std::min(3UL, results.size()); ++i)
		{
			const bankcli::Account* acc = std::get<const bankcli::Account*>(results[i]);
			std::cout << i + 1 << ") " << acc->name() << std::endl;
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

		const bankcli::Account* account = std::get<const bankcli::Account*>(results[num]);
		print_account(*account);
		break;
	}
}

void bankcli::new_account(bankcli::AccountBook& account_book)
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

		auto accounts = account_book.accounts();
		if (std::find_if(accounts.begin(), accounts.end(), [&ssn](const bankcli::Account& acc) { return acc.ssn() == ssn; }) != accounts.end())
		{
			std::cout << "Account already exists\n";
			return;
		}

		bankcli::Account* acc = account_book.add_accounts();
		acc->set_name(customer_name);
		acc->set_ssn(ssn);
		acc->set_creation_date(std::time(0));
		acc->set_id(counter++);

		validated = true;
		std::cout << "Account {" << acc->id() << "} created sucessfully!\n";
	}
}

void bankcli::close_account(bankcli::AccountBook& account_book)
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

	auto accounts = account_book.mutable_accounts();
	auto it = std::find_if(accounts->begin(), accounts->end(), [&num](const auto& acc)
	{
		return acc.id() == num;
	});

	if (it == accounts->end())
	{
		std::cout << "Invalid account" << std::endl;
		return;
	}

	std::string response = simplify(bankcli::prompt("Are you sure you want to close this account? Yes/No> "));
	if (response == "yes" || response == "y")
	{
		accounts->erase(it);
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
