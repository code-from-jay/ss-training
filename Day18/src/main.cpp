#include <cstring>
#include <cassert>
#include <utility>

template <typename T>
class myvector
{
	T* _data;
	int _size;
	int _capacity;
	T _undefined;

public:
	myvector() : _data(nullptr), _size(0), _capacity(0)
	{}

	myvector(T* elems, int sz)
	{
		// Allocate space for twice as many elements
		_data = new T[sz*2];
		_size = sz;
		_capacity = sz*2;

		memcpy(_data, elems, sizeof(T) * sz);
	}

	virtual ~myvector()
	{
		cleanup();
	}

	myvector(const myvector<T>& other)
	{
		_data = new T[other._capacity];
		_size = other._size;
		_capacity = other._capacity;

		memcpy(_data, other._data, sizeof(T) * _size);
	}

	myvector(myvector<T>&& other)
	{
		_data = std::exchange(other._data, nullptr);
		_size = std::exchange(other._size, 0);
		_capacity = std::exchange(other._capacity, 0);
	}

	myvector& operator=(const myvector<T>& other)
	{
		if (this == &other)
			return *this;

		cleanup();
		_data = new T[other._capacity];
		_size = other._size;
		_capacity = other._capacity;

		memcpy(_data, other._data, sizeof(T) * _size);

		return *this;
	}

	myvector& operator=(myvector<T>&& other)
	{
		if (this == &other)
			return *this;

		cleanup();
		_data = std::exchange(other._data, nullptr);
		_size = std::exchange(other._size, 0);
		_capacity = std::exchange(other._capacity, 0);

		return *this;
	}

	bool operator==(const myvector<T>& other)
	{
		if (_size != other._size)
			return false;

		for (int i = 0; i < _size; ++i)
		{
			if (_data[i] != other._data[i])
				return false;
		}

		return true;
	}

	T& operator[](int offset)
	{
		if (offset < 0 || offset >= _size)
			return _undefined;

		return _data[offset];
	}

	void push_back(const T& t)
	{
		if (_size == _capacity)
		{
			if (_capacity == 0) reserve(16);
			else reserve(_capacity * 2);
		}

		_data[_size] = t;
		_size++;
	}

	void pop_back()
	{
		if (_size == 0)
			return;
		_size--;
		_data[_size] = _undefined; // Make sure the object destructs
	}

	T& at(int offset)
	{
		return this->operator[](offset);
	}

	void clear()
	{
		for (int i = _size-1; i >= 0; --i)
			_data[i] = _undefined;
		_size = 0;
	}

	int size() const
	{
		return _size;
	}

	void reserve(int count)
	{
		assert(count > 0 && count >= _size);

		if (_data == nullptr)
		{
			_data = new T[count];
			_capacity = count;
			_size = 0;
			return;
		}

		T* newblock = new T[count];
		memcpy(newblock, _data, sizeof(T) * _size);
		delete[] _data;
		_data = newblock;
	}

	int capacity() const
	{
		return _capacity;
	}

	class iterator
	{
		friend myvector;

		myvector* _myvec;
		int _current;

	public:
		iterator(myvector* myvec, int offset) : _myvec(myvec), _current(offset) {}

		iterator& operator++()
		{
			_current++;
			return *this;
		}

		iterator operator++(int)
		{
			iterator old = *this;
			_current++;
			return old;
		}

		iterator& operator--()
		{
			_current--;
			return *this;
		}

		iterator operator--(int)
		{
			iterator old = *this;
			_current--;
			return old;
		}

		friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs._current == rhs._current; }
		friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
		friend bool operator<(const iterator& lhs, const iterator& rhs) { return lhs._current < rhs._current; }
		friend bool operator>(const iterator& lhs, const iterator& rhs) { return rhs < lhs; }
		friend bool operator<=(const iterator& lhs, const iterator& rhs) { return !(lhs > rhs); }
		friend bool operator>=(const iterator& lhs, const iterator& rhs) { return !(lhs < rhs); }

		T& operator*()
		{
			return _myvec->at(_current);
		}
	};

	iterator begin() const { return iterator(this, 0); }
	iterator end() const { return iterator(this, _size); }

private:
	void cleanup()
	{
		if (_data != nullptr)
		{
			delete[] _data;
			_data = nullptr;
		}
	}
};

int main()
{
	myvector<int> vec;
	assert(vec.size() == 0);
	vec.push_back(12);
	assert(vec.size() == 1);

	int arr[5] = { 1, 2, 3, 4, 5 };
	myvector<int> vec2{arr, 5};
	assert(vec2.size() == 5);
	assert(vec2[2] == 3);

	vec = std::move(vec2);
	assert(vec.size() == 5);
	assert(vec2.size() == 0);
	assert(vec[3] == 4);
}
