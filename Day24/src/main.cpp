#include <thread>
#include <mutex>
#include <queue>
#include <utility>
#include <functional>
#include <memory>
#include <random>
#include <iostream>

template<typename T>
class thread_safe_queue
{
	std::mutex mut;
	std::queue<T> data_queue;

public:
	T& front()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.front();
	}

	T& back()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.back();
	}

	void push(const T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(value);
	}

	void push(T&& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::forward<T>(value));
	}

	template<class... Args>
	decltype(auto) emplace(Args&&... args)
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.emplace(std::forward<Args>(args)...);
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = data_queue.front();
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}

	std::size_t size()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.size();
	}
};

enum class Operation
{
	Addition,
	Subtraction
};

struct MathOperation
{
	int a;
	int b;
	Operation operation;

	MathOperation(int a = 0, int b = 0, Operation operation = Operation::Addition) : a(a), b(b), operation(operation)
	{}
};

struct MathOperationResult
{
	int a;
	int b;
	Operation operation;
	int result;

	MathOperationResult(MathOperation mop, int result) : a(mop.a), b(mop.b), operation(mop.operation), result(result)
	{}
};

void handle_operations(thread_safe_queue<MathOperation>& work, thread_safe_queue<MathOperationResult>& results)
{
	while (true)
	{
		MathOperation mop;
		if (!work.try_pop(mop))
			break;
		switch (mop.operation)
		{
		case Operation::Addition:
			results.emplace(mop, mop.a + mop.b);
			break;
		case Operation::Subtraction:
			results.emplace(mop, mop.a - mop.b);
			break;
		}
	}
}

std::mutex io_lock;
void log_results(thread_safe_queue<MathOperationResult>& results)
{
	while (true)
	{
		auto result = results.try_pop();
		if (!result)
			break;

		io_lock.lock();
		std::cout << result->a << " " << (result->operation == Operation::Addition ? "+" : "-") << " " << result->b << " = " << result->result << std::endl;
		io_lock.unlock();
	}
}

int main()
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<int> operandsDist{1, 1000};
	std::uniform_int_distribution<char> operationDist{0, 1};

	auto rand_operand = std::bind(operandsDist, gen);
	auto rand_operation = std::bind(operationDist, gen);


	thread_safe_queue<MathOperation> work;
	for (int i = 0; i < 500; ++i)
	{
		work.emplace(rand_operand(), rand_operand(), rand_operation() == 0 ? Operation::Addition : Operation::Subtraction);
	}

	thread_safe_queue<MathOperationResult> results;

	std::thread t1{handle_operations, std::ref(work), std::ref(results)};
	std::thread t2{handle_operations, std::ref(work), std::ref(results)};
	std::thread t3{handle_operations, std::ref(work), std::ref(results)};

	t1.join();
	t2.join();
	t3.join();

	t1 = std::thread{log_results, std::ref(results)};
	t2 = std::thread{log_results, std::ref(results)};
	t3 = std::thread{log_results, std::ref(results)};

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
