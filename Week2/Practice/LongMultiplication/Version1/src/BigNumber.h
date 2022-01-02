#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

class BigNumber
{
	long _number;
public:
	BigNumber(long input);

	void set(long input);
	long number() const;
	void multiply(const BigNumber& input, BigNumber& output) const;
};

#endif
