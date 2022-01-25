#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <functional>

enum class CurrencyType
{
	Dollars,
	Pounds
};

enum class TransactionType
{
	Buy,
	Sell
};

enum class TransactionResult
{
	OK, // Transaction accepted and fully processed
	Cancel_SenderInsuffFunds, // Transaction canceled because the sender did not have enough funds
	Cancel_RecipientInsuffFunds, // Transaction canceled because the recipient did not have enough funds
	Cancel_BadExRate // Transaction canceled because the recipient did not like the exchange rate
};

struct Request
{
	double exchangeRate;
	long amount;
	CurrencyType currencyType;
	TransactionType transactionType;

	Request(double exchangeRate, long amount, CurrencyType currencyType, TransactionType transactionType) :
		exchangeRate(exchangeRate), amount(amount), currencyType(currencyType), transactionType(transactionType)
	{}
};

bool continue_running = true;

struct Country
{
	long dollars;
	long pounds;
	std::mutex mutex;

	Country(long dollars, long pounds) : dollars(dollars), pounds(pounds), mutex() {}

	TransactionResult process_request(const Request& request)
	{
		std::lock_guard lock(mutex);

		if (request.transactionType == TransactionType::Buy)
		{
			if (request.currencyType == CurrencyType::Dollars)
			{	if (request.amount > dollars) return TransactionResult::Cancel_RecipientInsuffFunds;
				if (request.exchangeRate >= 2.0) return TransactionResult::Cancel_BadExRate;

				dollars -= request.amount;
				pounds += request.amount * request.exchangeRate;
			}
			else if (request.currencyType == CurrencyType::Pounds)
			{
				if (request.amount > pounds) return TransactionResult::Cancel_RecipientInsuffFunds;
				if (request.exchangeRate >= 2.0) return TransactionResult::Cancel_BadExRate;

				pounds -= request.amount;
				dollars += request.amount * request.exchangeRate;
			}
		}
		else if (request.transactionType == TransactionType::Sell)
		{
			if (request.currencyType == CurrencyType::Dollars)
			{
				if (request.exchangeRate <= 0.5) return TransactionResult::Cancel_BadExRate;
				if (request.amount * request.exchangeRate > pounds) return TransactionResult::Cancel_RecipientInsuffFunds;

				dollars += request.amount;
				pounds -= request.amount * request.exchangeRate;
			}
			else if (request.currencyType == CurrencyType::Pounds)
			{
				if (request.exchangeRate <= 0.5) return TransactionResult::Cancel_BadExRate;
				if (request.amount * request.exchangeRate > dollars) return TransactionResult::Cancel_RecipientInsuffFunds;

				pounds += request.amount;
				dollars -= request.amount * request.exchangeRate;
			}
		}

		return TransactionResult::OK;
	}

	TransactionResult send_request(Country& recipient, const Request& request)
	{
		{
			std::lock_guard lock(mutex);
			if (request.transactionType == TransactionType::Buy &&
			  ((request.currencyType == CurrencyType::Dollars &&
			    request.amount * request.exchangeRate > pounds) || 
			   (request.currencyType == CurrencyType::Pounds &&
			    request.amount * request.exchangeRate > dollars)))
			{
				return TransactionResult::Cancel_SenderInsuffFunds;
			}
			else if (request.transactionType == TransactionType::Sell &&
			  ((request.currencyType == CurrencyType::Dollars &&
			    request.amount > dollars) ||
			   (request.currencyType == CurrencyType::Pounds &&
			    request.amount > pounds)))
			{
				return TransactionResult::Cancel_SenderInsuffFunds;
			}
		}

		TransactionResult result = recipient.process_request(request);
		if (result != TransactionResult::OK)
			return result;

		std::lock_guard lock(mutex);
		if (request.transactionType == TransactionType::Buy && request.currencyType == CurrencyType::Dollars)
		{
			dollars += request.amount;
			pounds -= request.amount * request.exchangeRate;
		}
		else if (request.transactionType == TransactionType::Buy && request.currencyType == CurrencyType::Pounds)
		{
			pounds += request.amount;
			dollars -= request.amount * request.exchangeRate;
		}
		else if (request.transactionType == TransactionType::Sell && request.currencyType == CurrencyType::Dollars)
		{
			dollars -= request.amount;
			pounds += request.amount * request.exchangeRate;
		}
		else if (request.transactionType == TransactionType::Sell && request.currencyType == CurrencyType::Pounds)
		{
			pounds -= request.amount;
			dollars += request.amount * request.exchangeRate;
		}

		return TransactionResult::OK;
	}
};

void handle_transactions(const std::string& thread_name, Country& self, Country& other, const std::vector<Request>& requestPool)
{
	using std::chrono::system_clock;
	std::time_t tt;

	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<std::size_t> distribution{0, requestPool.size()-1};
	auto rand_index = std::bind(distribution, gen);

	std::ofstream log{thread_name + ".log", std::ios::app};

	while (continue_running)
	{
		auto& request = requestPool[rand_index()];
		TransactionResult result = self.send_request(other, request);
		auto now = system_clock::now();
		tt = system_clock::to_time_t(now);
		log
			<< "Request { exchangeRate: " << request.exchangeRate
			<< ", amount: " << request.amount
			<< ", currencyType: " << (request.currencyType == CurrencyType::Dollars ? "Dollars" : "Pounds")
			<< ", transactionType: " << (request.transactionType == TransactionType::Buy ? "BUY" : "SELL") << " }: ";
		switch (result)
		{
		case TransactionResult::OK:
			log << "OK!\n";
			break;
		case TransactionResult::Cancel_SenderInsuffFunds:
			log << "Sender doesn't have enough funds!\n";
			break;
		case TransactionResult::Cancel_RecipientInsuffFunds:
			log << "Recipient doesn't have enough funds!\n";
			break;
		case TransactionResult::Cancel_BadExRate:
			log << "Recipient refuses supplied exchange rate!\n";
			break;
		}

		// Just so it's visually clear in the console whenever a deadlock happens
		std::cout << thread_name << " processed transaction!\n";
	}
}

int main(int argc, char** argv)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<long> currencyDist{100000, 300000};
	std::uniform_int_distribution<char> boolDist{0, 1};
	std::uniform_real_distribution<double> exchangeDist{0.2, 2.8};

	auto rand_currency = std::bind(currencyDist, gen);
	auto rand_boolean  = std::bind(boolDist, gen);
	auto rand_exchange = std::bind(exchangeDist, gen);

	std::vector<Request> requestPool;
	for (int i = 0; i < 10; ++i)
	{
		auto exchangeRate = rand_exchange();
		auto amount = rand_currency();
		auto currencyType = rand_boolean() == 0 ? CurrencyType::Dollars : CurrencyType::Pounds;
		auto transactionType = rand_boolean() == 0 ? TransactionType::Buy : TransactionType::Sell;

		requestPool.emplace_back(exchangeRate, amount, currencyType, transactionType);
	}

	std::cout << "Press any key to stop transactions!" << std::endl;

	Country c1{500000, 500000};
	Country c2{500000, 500000};
	std::thread t1{handle_transactions, "United States of America", std::ref(c1), std::ref(c2), std::ref(requestPool)};
	std::thread t2{handle_transactions, "Great Britain", std::ref(c2), std::ref(c1), std::ref(requestPool)};

	std::this_thread::sleep_for(std::chrono::seconds(10));

	continue_running = false;
	t1.join();
	t2.join();

	std::cout << "Program successfully ran for 10 seconds!\n";

	return 0;
}
