#include <cmath>
#include <iostream>

#include <bits/stdc++.h>

const double phi = (1.0 + std::pow(5, 0.5))/2.0;
const double psi = 1-phi;

int nth_fib(int n)
{
	return (int)trunc((pow(phi, n) - pow(psi, n)) / (phi - psi));
}

int fib_index_less_than_N(int N)
{
	return (int)floor(log(N*sqrt(5))/(log(1+sqrt(5))-log(2)));
}


int main(void)
{
	int index = fib_index_less_than_N(INT_MAX);
	std::cout << "The largest Fibonacci number that fits in a 32-bit int is index "
		  << index << " which gives the number '" << nth_fib(index) << "'\n";
	return 0;
}
