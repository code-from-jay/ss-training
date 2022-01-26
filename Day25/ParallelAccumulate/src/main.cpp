#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <cassert>

template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, std::promise<T> acc_promise)
	{
		T result = std::accumulate(first, last, 0);
		acc_promise.set_value(result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return init;

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	std::cout << "num_threads: " << num_threads << std::endl;
	
	unsigned long const block_size = length/num_threads;
	std::vector<std::future<T>> results(num_threads);
	std::vector<std::thread> threads(num_threads-1);
	Iterator block_start = first;

	auto resolve_and_add = [](T a, std::future<T>& b)
	{
		return a + b.get();
	};

	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		std::promise<T> acc_promise;
		results[i] = acc_promise.get_future();

		threads[i] = std::thread(accumulate_block<Iterator,T>(), block_start, block_end, std::move(acc_promise));
		block_start = block_end;
	}

	std::promise<T> fin_promise;
	results[num_threads-1] = fin_promise.get_future();

	accumulate_block<Iterator, T>()(block_start, last, std::move(fin_promise));

	T result = std::accumulate(results.begin(), results.end(), init, resolve_and_add);

	for (auto& entry : threads)
		entry.join();

	return result;
}

int main()
{
	constexpr long LEN = 20000;
	std::vector<std::size_t> myvec;
	for (long ii = 0; ii < LEN; ++ii)
		myvec.push_back(ii);
	std::size_t sum = parallel_accumulate(myvec.begin(), myvec.end(), std::size_t(0));
	std::cout << "sum: " << sum << std::endl;
	assert(sum == 199990000);
	return 0;
}
