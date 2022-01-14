#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <type_traits>
#include <utility>

template <class T, class C>
class Account
{
	using BaseCurrency = T;
	using Conversion = C;
	using TAccount = Account<BaseCurrency, Conversion>; // Short hand for writing this specification of Account

	Conversion conversion;
	BaseCurrency amt;
public:
	Account() = default;

	Account(BaseCurrency amount) : amt(amount)
	{}

	Account(const TAccount& other) : amt(other.amt)
	{}

	Account(TAccount&& other) : amt(std::move(other.amt))
	{}

	TAccount& operator=(const TAccount& other)
	{
		if (this == &other)
			return *this;

		amt = other.amt;
		return *this;
	}

	TAccount& operator=(TAccount&& other)
	{
		if (this == &other)
			return *this;

		amt = std::move(other.amt);
		return *this;
	}

	template <class Q>
	BaseCurrency deposit(Q amount)
	{
		BaseCurrency cur;
		conversion(amount, cur);
		this->amt += cur.amount;
		return this->amt;
	}

	template <class Q>
	BaseCurrency withdraw(Q amount)
	{
		BaseCurrency cur;
		conversion(amount, cur);
		this->amt -= cur.amount;
		return this->amt;
	}

	template <class R>
	Account<R, Conversion> currency() const
	{
		R cur;
		conversion(this->amt, cur);
		return Account<R, Conversion>(cur);
	}

	BaseCurrency amount() const
	{
		return this->amt;
	}

	template <class X>
	friend bool operator==(const TAccount& lhs, const Account<X, Conversion>& rhs)
	{
		if (std::is_same_v<TAccount, Account<X, Conversion>>)
			return lhs.amt == rhs.amt;

		BaseCurrency rhsAmt;
		Conversion con;
		con(rhs.amt, rhsAmt);
		return lhs.amt == rhsAmt;
	}
	friend bool operator!=(const TAccount& lhs, const TAccount& rhs) { return !(lhs == rhs); }
	friend bool operator<(const TAccount& lhs, const TAccount& rhs) { return lhs.amt < rhs.amt; }
	friend bool operator>(const TAccount& lhs, const TAccount& rhs) { return rhs < lhs; }
	friend bool operator<=(const TAccount& lhs, const TAccount& rhs) { return !(lhs > rhs); }
	friend bool operator>=(const TAccount& lhs, const TAccount& rhs) { return !(lhs < rhs); }

	TAccount& operator++() { this->amt++; return *this; }
	TAccount operator++(int) { TAccount old = *this; operator++(); return old; }
	TAccount& operator--() { this->amt--; return *this; }
	TAccount operator--(int) { TAccount old = *this; operator--(); return old; }

	template <typename X>
	TAccount& operator+=(X rhs)
	{
		static_assert(std::is_arithmetic_v<X>);
		this->amt += rhs;
		return *this;
	}

	template <typename X>
	TAccount& operator-=(X rhs)
	{
		static_assert(std::is_arithmetic_v<X>);
		this->amt -= rhs;
		return *this;
	}

	template <typename X>
	TAccount& operator*=(X rhs)
	{
		static_assert(std::is_arithmetic_v<X>);
		this->amt *= rhs;
		return *this;
	}

	template <typename X>
	TAccount& operator/=(X rhs)
	{
		static_assert(std::is_arithmetic_v<X>);
		this->amt /= rhs;
		return *this;
	}

	template <typename X>
	TAccount& operator%=(X rhs)
	{
		static_assert(std::is_arithmetic_v<X>);
		this->amt %= rhs;
		return *this;
	}

	template <typename X> friend TAccount operator+(TAccount lhs, X rhs) { lhs += rhs; return lhs; }
	template <typename X> friend TAccount operator-(TAccount lhs, X rhs) { lhs -= rhs; return lhs; }
	template <typename X> friend TAccount operator*(TAccount lhs, X rhs) { lhs *= rhs; return lhs; }
	template <typename X> friend TAccount operator/(TAccount lhs, X rhs) { lhs /= rhs; return lhs; }
	template <typename X> friend TAccount operator%(TAccount lhs, X rhs) { lhs %= rhs; return lhs; }

	template <class Q>
	operator Account<Q, Conversion>()
	{
		Q cur;
		conversion(this->amt, cur);
		return Account<Q, Conversion>(cur);
	}
};

#endif // ACCOUNT_H
