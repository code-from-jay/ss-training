#include <coroutine>
#include <memory>
#include <iostream>
#include <utility>
#include <random>
#include <chrono>
#include <sstream>

enum transaction_type : unsigned char
{
	CREDIT = 0,
	DEBIT = 1
};

struct transaction
{
	long post_date;
	std::string src_accountId;
	std::string dest_accountId;
	transaction_type tranType;
	double amount;
};

class TransactionGenerator
{
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
private:
	handle_type _handle;
	
public:
	struct promise_type
	{
		transaction value;

		TransactionGenerator get_return_object()
		{
			return TransactionGenerator(handle_type::from_promise(*this));
		}
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void unhandled_exception() {}
		std::suspend_always yield_value(transaction value)
		{
			this->value = value;
			return {};
		}
	};

	TransactionGenerator(handle_type handle) : _handle(handle) {}
	~TransactionGenerator() { if (_handle) _handle.destroy(); }

	transaction next()
	{
		transaction ret = _handle.promise().value;
		_handle();
		return ret;
	}

	static TransactionGenerator create()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned char> boolDis(0, 1);
		std::uniform_real_distribution<double> curDis(1,1000000);
		while (true)
		{
			const auto now = std::chrono::system_clock::now();
			transaction ret;
			ret.post_date = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
			ret.src_accountId = generate_uuid();
			ret.dest_accountId = generate_uuid();
			ret.tranType = (transaction_type)boolDis(gen);
			ret.amount = curDis(gen);

			co_yield ret;
		}
	};

private:
	static std::string generate_uuid()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dis(0, 15);
		static std::uniform_int_distribution<int> dis2(8, 11);

		std::stringstream ss;
		int i;
		ss << std::hex;
		for (i = 0; i < 8; ++i)
			ss << dis(gen);
		ss << "-";
		for (i = 0; i < 4; ++i)
			ss << dis(gen);
		ss << "-4";
		for (i = 0; i < 3; ++i)
			ss << dis(gen);
		ss << "-";
		ss << dis2(gen);
		for (i = 0; i < 3; ++i)
			ss << dis(gen);
		ss << "-";
		for (i = 0; i < 12; ++i)
			ss << dis(gen);
		return ss.str();
	}
};

int main()
{
	auto generator = TransactionGenerator::create();
	for (int i = 0; i < 10; ++i)
	{
		auto transaction = generator.next();
		std::cout << "Transaction:"
			<< "\n\tPost Timestamp: " << transaction.post_date
			<< "\n\tSource Account: " << transaction.src_accountId
			<< "\n\tDestination Account: " << transaction.dest_accountId
			<< "\n\tTransaction Type: " << (transaction.tranType == CREDIT ? "Credit" : "Debit")
			<< "\n\tAmount: " << transaction.amount << "\n";
	}

	return 0;
}
