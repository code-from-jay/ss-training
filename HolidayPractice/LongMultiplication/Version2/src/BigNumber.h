#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <string>
#include <vector>

typedef std::vector<std::vector<unsigned char>> numvec;

class BigNumber
{
	bool _overflown;
	union
	{
		long _number;
		unsigned char _numstr[64];
	};
public:
	BigNumber(long input);
	BigNumber(const std::string& input);

	void set(long input);
	void set(const std::string& input);

	std::string number() const;

	BigNumber multiply(const BigNumber& input) const;
	numvec simulate_multiply(const BigNumber& input) const;

	void print(const numvec& myvec) const;
};

#define vtos(char_vec) std::string{(char_vec).begin(), (char_vec).end()}
#define stov(str) std::vector<unsigned char>{(str).begin(), (str).end()}

#endif
