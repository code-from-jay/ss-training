#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <cassert>

template <typename RandomIt>
void parallel_sum(RandomIt beg, RandomIt end, std::promise<std::size_t> promise)
{
	auto len = end - beg;
	if (len < 1000)
	{
		promise.set_value(std::accumulate(beg, end, 0));
		return;
	}

	RandomIt mid = beg + len / 2;
	std::promise<std::size_t> inner_promise;
	std::future<std::size_t> inner = inner_promise.get_future();
	auto handle = std::async(std::launch::async, parallel_sum<RandomIt>, mid, end, std::move(inner_promise));

	std::promise<std::size_t> outer_promise;
	std::future<std::size_t> outer = outer_promise.get_future();
	parallel_sum(beg, mid, std::move(outer_promise));
	promise.set_value(inner.get() + outer.get());
}

int main()
{
	constexpr long LEN = 20000;
	std::vector<std::size_t> myvec;
	for (long ii = 0; ii < LEN; ++ii)
		myvec.push_back(ii);

	std::promise<std::size_t> result_promise;
	std::future<std::size_t> result = result_promise.get_future();
	parallel_sum(myvec.begin(), myvec.end(), std::move(result_promise));

	assert(result.get() == 199990000);
	return 0;
}
