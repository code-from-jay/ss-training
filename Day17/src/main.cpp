#include "account.h"

#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>

/**
 * For currencies that have been decimalized, provided 2 extra digits of precision
 */
struct DecimalCurrency
{
	using DC = DecimalCurrency; // Short hand

	long amount;
	std::string code;

	DecimalCurrency(long amount, std::string code) : amount(amount), code(code) {}
	
	DC& operator++() { amount += 100; return *this; }
	DC& operator--() { amount -= 100; return *this; }
	DC operator++(int) { DC old = *this; operator++(); return old; }
	DC operator--(int) { DC old = *this; operator--(); return old; }

	template <typename X>
	DC& operator+=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount += rhs * 100; return *this; }
	template <typename X>
	DC& operator-=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount -= rhs * 100; return *this; }
	template <typename X>
	DC& operator*=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount *= rhs; return *this; }
	template <typename X>
	DC& operator/=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount /= rhs; return *this; }
	template <typename X>
	DC& operator%=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount %= rhs; return *this; }

	template <typename X> friend DC operator+(DC lhs, X rhs) { lhs += rhs; return lhs; }
	template <typename X> friend DC operator-(DC lhs, X rhs) { lhs -= rhs; return lhs; }
	template <typename X> friend DC operator*(DC lhs, X rhs) { lhs *= rhs; return lhs; }
	template <typename X> friend DC operator/(DC lhs, X rhs) { lhs /= rhs; return lhs; }
	template <typename X> friend DC operator%(DC lhs, X rhs) { lhs %= rhs; return lhs; }

	std::string to_string()
	{
		int unit = amount / 10000;
		int subunit = (amount % 10000) / 100;

		std::stringstream ss;
		ss << code << " " << unit << "." << std::setw(2) << std::setfill('0') << subunit;
		return ss.str();
	}
};

struct Dollar : public DecimalCurrency
{
	Dollar(long amount = 0) : DecimalCurrency(amount, "USD") {}
};
struct Euro : public DecimalCurrency
{
	Euro(long amount = 0) : DecimalCurrency(amount, "EUR") {}
};
struct Pound : public DecimalCurrency
{
	Pound(long amount = 0) : DecimalCurrency(amount, "GBP") {}
};

struct Yen
{
	long amount;

	Yen& operator++() { amount++; return *this; }
	Yen& operator--() { amount--; return *this; }
	Yen operator++(int) { Yen old = *this; operator++(); return old; }
	Yen operator--(int) { Yen old = *this; operator--(); return old; }

	template <typename X> Yen& operator+=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount += rhs; return *this; }
	template <typename X> Yen& operator-=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount -= rhs; return *this; }
	template <typename X> Yen& operator*=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount *= rhs; return *this; }
	template <typename X> Yen& operator/=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount /= rhs; return *this; }
	template <typename X> Yen& operator%=(X rhs) { static_assert(std::is_arithmetic_v<X>); amount %= rhs; return *this; }

	template <typename X> friend Yen operator+(Yen lhs, X rhs) { lhs += rhs; return lhs; };
	template <typename X> friend Yen operator-(Yen lhs, X rhs) { lhs -= rhs; return lhs; };
	template <typename X> friend Yen operator*(Yen lhs, X rhs) { lhs *= rhs; return lhs; };
	template <typename X> friend Yen operator/(Yen lhs, X rhs) { lhs /= rhs; return lhs; };
	template <typename X> friend Yen operator%(Yen lhs, X rhs) { lhs %= rhs; return lhs; };

	std::string to_string()
	{
		std::stringstream ss;
		ss << "JPY " << amount;
		return ss.str();
	}
};

struct Converter
{
	void operator()(const Dollar& base, Dollar& amt) { amt.amount = base.amount; }
	void operator()(const Dollar& base, Euro& amt)   { amt.amount = base.amount * 0.87308431; }
	void operator()(const Dollar& base, Pound& amt)  { amt.amount = base.amount * 0.72956572; }
	void operator()(const Dollar& base, Yen& amt)    { amt.amount = base.amount * 1.1413236; }

	void operator()(const Euro& base, Dollar& amt) { amt.amount = base.amount * 1.145351; }
	void operator()(const Euro& base, Euro& amt)   { amt.amount = base.amount; }
	void operator()(const Euro& base, Pound& amt)  { amt.amount = base.amount * 0.83557751; }
	void operator()(const Euro& base, Yen& amt)    { amt.amount = base.amount * 1.3072654; }

	void operator()(const Pound& base, Dollar& amt) { amt.amount = base.amount * 1.3707377; }
	void operator()(const Pound& base, Euro& amt)   { amt.amount = base.amount * 1.1967127; }
	void operator()(const Pound& base, Pound& amt)  { amt.amount = base.amount; }
	void operator()(const Pound& base, Yen& amt)    { amt.amount = base.amount * 1.5643631; }

	void operator()(const Yen& base, Dollar& amt) { amt.amount = base.amount * 0.87622795; }
	void operator()(const Yen& base, Euro& amt)   { amt.amount = base.amount * 0.76498599; }
	void operator()(const Yen& base, Pound& amt)  { amt.amount = base.amount * 0.63921713; }
	void operator()(const Yen& base, Yen& amt)    { amt.amount = base.amount; }
};

template <class T>
using CAccount = Account<T, Converter>;

int main()
{
	CAccount<Dollar> USDAccount = Dollar{350000}; // 35 dollars
	CAccount<Euro> EURAccount = Euro{150000}; // 15 euros
	CAccount<Pound> GBPAccount = Pound{2000000}; // 200 pounds
	CAccount<Yen> JPYAccount = Yen{18400}; // 18,400 yen

	CAccount<Dollar> emptyAccount;
	assert(emptyAccount.amount().amount == 0);
	assert(emptyAccount.deposit(JPYAccount.amount()).amount == emptyAccount.amount().amount);

	std::cout << emptyAccount.amount().to_string() << std::endl;

	return 0;
}
