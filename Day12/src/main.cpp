#include <cassert>
#include <cstring>
#include <utility>

class MyString
{
	char* _buffer;
	size_t _buflen;
public:
	// Makes a copy of the passed string
	MyString(const char* string)
	{
		if (string == nullptr)
		{
			_buflen = 0;
			_buffer = nullptr;
			return;
		}

		_buflen = strlen(string) + 1; // +1 because strlen doesn't count the null terminator
		_buffer = new char[_buflen];
		strcpy(_buffer, string);
	}

	// Default constructor, essentially the equivalent of passing nullptr to MyString(const char*)
	MyString()
	{
		_buflen = 0;
		_buffer = nullptr;
	}

	// Copy constructor
	MyString(const MyString& other)
	{
		_buflen = other._buflen;
		_buffer = new char[_buflen];
		strcpy(_buffer, other._buffer);
	}

	// Move constructor, steals the pointer to other's buffer
	MyString(MyString&& other)
	{
		_buffer = std::exchange(other._buffer, nullptr);
		_buflen = std::exchange(other._buflen, 0);
	}

	// Copy assignment, ditto of copy constructor, but cleans up anything that was already in the class
	MyString& operator=(const MyString& other)
	{
		if (this == &other)
			return *this;

		cleanup();
		_buflen = other._buflen;
		_buffer = new char[_buflen];
		strcpy(_buffer, other._buffer);
		return *this;
	}

	// Move assignment, ditto of move constructor, but cleans up anything that was already in the class
	MyString& operator=(MyString&& other)
	{
		if (this == &other)
			return *this;

		cleanup();
		_buffer = std::exchange(other._buffer, nullptr);
		_buflen = std::exchange(other._buflen, 0);
		return *this;
	}

	~MyString()
	{
		cleanup();
	}

	// This allows you to actually modify the buffer, but removing/modify the null terminator is undefined behaviour
	char& operator[](size_t idx)
	{
		assert(idx <= _buflen);
		
		return _buffer[idx];
	}

	// Will only allow for reading of the buffer, and not modifying
	const char& operator[](size_t idx) const
	{
		assert(idx <= _buflen);
		return _buffer[idx];
	}

	// Appends a string to this class in place
	MyString& operator+=(const MyString& rhs)
	{
		size_t newlen = _buflen + rhs._buflen - 1;
		char* newbuf = new char[newlen];
		strcpy(newbuf, _buffer);
		strcat(newbuf, rhs._buffer);
		cleanup();
		_buffer = newbuf;
		_buflen = newlen;
		return *this;
	}

	/**
	 * I'm making a copy of `lhs` so I can just reuse my += operator
	 * without modifying any of the values placed in.
	 */
	friend MyString operator+(MyString lhs, const MyString& rhs)
	{
		return lhs += rhs;
	}

	// Returns a section of the string, specifically buffer[offset:] in python terms
	friend MyString operator+(const MyString& lhs, size_t offset)
	{
		assert(offset < lhs._buflen);

		return MyString(lhs._buffer+1);
	}

	/**
	 * The equivalent of Python's * operator for strings,
	 * one thing to note is that passing 0 will give you a *valid*
	 * empty string, meaning that the buffer is 1 element large, and contains
	 * only a null terminator character.
	 */
	friend MyString operator*(const MyString& lhs, int repeats)
	{
		if (repeats == 0)
			return MyString("");

		size_t newlen = ((lhs._buflen - 1) * 3) + 1;
		char* newbuf = new char[newlen];
		newbuf[0] = '\0';
		for (int i = 0; i < repeats; ++i)
		{
			strcat(newbuf, lhs._buffer);
		}
		MyString res;
		res._buffer = newbuf;
		res._buflen = newlen;
		return res;
	}

	// Compares the 2 buffers and only checks if they are exactly the same
	friend bool operator==(const MyString& lhs, const MyString& rhs)
	{
		return strcmp(lhs._buffer, rhs._buffer) == 0;
	}

	// Test method for assertions
	bool initialized()
	{
		return _buffer != nullptr;
	}

private:
	// frees up used memory
	void cleanup()
	{
		if (_buffer)
		{
			delete[] _buffer;
			_buffer = nullptr;
		}
		_buflen = 0;
	}
};

int main(void)
{
	MyString s = "abc";
	assert((s + s) == "abcabc");
	assert((s * 3) == "abcabcabc");
	assert(s[2] == 'c');
	assert((s + 1) == "bc");

	MyString b = std::move(s);
	assert(!s.initialized());
	assert(b == "abc");
	return 0;
}
