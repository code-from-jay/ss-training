# Operators

## Unary Operators

`++x`, `x++`, `--x`, `x--`, `~x`, `!x`, and `x()`.

These are the Pre-Increment, Post-Increment, Pre-Decrement, Post-Decrement, Bitwise Inverse, NOT, and the call operators respectively.

The Increment and Decrement operators modify the variable they're used on, so they're really good in loops, like so.

```cpp
for (int x = 0; x < 5; ++x)
{
	std::cout << "X: " << x << std::endl;
}

/**
 * Output:
 *  X: 0
 *  X: 1
 *  X: 2
 *  X: 3
 *  X: 4
 */
```

The Bitwise Inverse operator, as the name implies, will invert all the bits of a value, so 3 (00000011) for example will become 252 (11111100).

The NOT operator is a boolean operator, so it inverts the 'truthy'-ness of a value, which is to say it'll turn any non-zero value (a.k.a. True) to 0 (a.k.a. False), and vise versa. It's really useful for when you want to check if a value is false in an if statement, like so:

```cpp
bool started = false;

if (!started)
{
	// Do some startup initialization
	started = true;
}
```

The call operator is mainly used whenever you're calling a function/method, like so:

```cpp
#include <cstdio>

void test(void)
{
	printf("Hello!\n");
}

typedef void(*test_ptr)(void);

int main(void)
{
	// Using the call operator on a
	// function name.
	test();

	// We call also use the call operator
	// on a variable!
	test_ptr test_func = test;
	test_func();

	return 0;
}
```

## Binary Operators

| Name | Operator |
| ---- | -------- |
| Assignment | `x = y` |
| Addition | `x + y` |
| Subtraction | `x - y` |
| Multiplication | `x * y` |
| Division | `x / y` |
| Modulo | `x % y` |
| Bitwise AND | `x & y` |
| Bitwise OR | `x | y` |
| Bitwise XOR | `x ^ y` |
| Left Bitshift | `x << y` |
| Right Bitshift | `x >> y` |
| Inline Addition | `x += y` |
| Inline Subtraction | `x -= y` |
| Inline Multiplication | `x *= y` |
| Inline Division | `x /= y` |
| Inline Modulo | `x %= y` |
| Inline Bitwise AND | `x &= y` |
| Inline Bitwise OR | `x |= y` |
| Inline Bitwise XOR | `x ^= y` |
| Inline Left Bitshift | `x <<= y` |
| Inline Right Bitshift | `x >>= y` |
| Less-Than | `x < y` |
| Greater-Than | `x > y` |
| Less-Than-Equal | `x <= y` |
| Greater-Than-Equal | `x >= y` |
| Equality | `x == y` |
| Non-Equality | `x != y` |
| Logical AND | `x && y` |
| Logical OR | `x || y` | 

The binary operators are fairly self-explanatory, they're used whenever you want to perform arithmetic or do comparisons, here's an example who how to use some of them:

```cpp
int x = 9;
int y = 10;

if (x + y < 20)
{
	std::cout << "X + Y is less than 20!" << std::endl;
}
```

# Ternary Operator

The Ternary Operator (`x ? y : z`) is used for inline condition checking, it's used like so `<condition> ? <truth-expression> : <false-expression>` this is really good for when you want to assign a value based on a small condition, for example:

```cpp
int x = 20;
int y = 15;

// This would choose the biggest of the two
// values, which in this case would be x!
// Making `biggest_value` equal to 20!
int biggest_value = x > y ? x : y;
```
