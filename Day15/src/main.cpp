#include <exception>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cassert>

class IntRepresentationError : std::exception
{
	float _value;
	std::string _what;
public:
	IntRepresentationError(float value) : _value(value)
	{
		std::stringstream ss;
		ss << "Float value '" << _value << "' is not representable as an Int!";
		_what = ss.str();
	}

	const char* what() const noexcept override
	{
		return _what.c_str();
	}
};

/**
 * Returns:
 *   a <  b: -1
 *   a == b:  0
 *   a >  b:  1
 * Throws:
 *   IntRepresentationError if a or b aren't exactly representable
 *   as an integer.
 */
int compare(float a, float b) noexcept
{
	if (truncf(a) != a)
		throw IntRepresentationError{a};
	if (truncf(b) != b)
		throw IntRepresentationError{b};

	return a < b ? -1 : a > b ? 1 : 0;
}

int main()
{
	assert(compare(6.0f, 5.0f) == 1);
	assert(compare(1.0f, 2.0f) == -1);
	assert(compare(5.0f, 5.0f) == 0);

	try
	{
		compare(2.5f, 1.0f);
	}
	catch (...)
	{
		// This should never execute, an exception from compare should throw std::terminate
		assert(false);
	}
}
