#pragma once

#include <type_traits>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <stdexcept>

template <class T>
class stack
{
public:
	using value_type = T;
	using size_type = std::size_t;

	stack() noexcept;
	~stack() noexcept;

	stack(const stack&) noexcept(std::is_nothrow_copy_constructible<T>::value);
	stack& operator=(stack) noexcept(std::is_nothrow_copy_constructible<T>::value);
	
	stack(stack&&) noexcept(std::is_nothrow_move_constructible<T>::value);
	stack& operator=(stack&&) noexcept(std::is_nothrow_move_constructible<T>::value);

	size_type size() const noexcept { return _size; }
	size_type capacity() const noexcept { return _capacity; }
	bool empty() const noexcept { return size() == 0; }

	void push(const T&);
	template <class... Args> void emplace(Args&&...);
	T pop();

	void reserve(size_type);

private:
	void swap(stack<T>&);

	static T* _copy(T*, size_type, size_type);
	static T* _move(T*, size_type, size_type);
	static void _destroy(T*, size_type);

	T* _data = nullptr;
	size_type _capacity = 0;
	size_type _size = 0;
};

template <class T>
stack<T>::stack() noexcept
{}

template <class T>
stack<T>::~stack() noexcept
{
	if (_data)
	{
		_destroy(_data, _size);
		_data = nullptr;
	}
}

template <class T>
stack<T>::stack(const stack& s) 
noexcept(std::is_nothrow_copy_constructible<T>::value)
{
	_data = _copy(s._data, s._size, s._capacity);

	assert(_data);
	_capacity = s._capacity;
	_size = s._size;
}

template <class T>
void stack<T>::swap(stack<T>& other) 
{
	using std::swap;
	swap(_data, other._data);
	swap(_size, other._size);
	swap(_capacity, other._capacity);
}

template <class T>
stack<T>& stack<T>::operator=(stack<T> s)
noexcept(std::is_nothrow_copy_constructible<T>::value)
{
	swap(s);
	return *this;
}

template <class T>
stack<T>::stack(stack&& s) 
noexcept(std::is_nothrow_move_constructible<T>::value)
{
	swap(s);
}

template <class T>
stack<T>& stack<T>::operator=(stack<T>&& s)
noexcept(std::is_nothrow_move_constructible<T>::value)
{
	swap(s);
	return *this;
}

template <class T>
void stack<T>::push(const T& t) 
{
	if (_size == _capacity)
	{
		reserve((_capacity + 1) * 2);
	}

	assert(_capacity > _size);

	_data[_size] = t;
	++_size;
}

template <class T>
template <class... Args>
void stack<T>::emplace(Args&&... args)
{
	if (_size == _capacity)
	{
		reserve((_capacity + 1) * 2);
	}

	assert(_capacity > _size);

	new (&_data[_size]) T(std::forward<Args>(args)...);
	++_size;
}

template <class T>
void stack<T>::reserve(size_type new_capacity) 
{
	assert(new_capacity >= _capacity);
	T* new_data = _move(_data, _size, new_capacity);

	if (_data)
	{
		assert(_size > 0);
		_destroy(_data, _size);
	}

	_data = new_data;
	_capacity = new_capacity;
}

template <class T>
T stack<T>::pop() 
{
	if (empty())
	{
		throw std::runtime_error("empty container");
	}

	const T elem = _data[_size - 1];
	--_size;
	
	return elem;
}

template <class T>
T* stack<T>::_copy(T* src, size_type size, size_type capacity) 
{
	T* buff = static_cast<T*>(std::malloc(sizeof(T) * capacity));
	size_type i;
	try
	{
		for (i = 0; i < size; ++i)
		{
			new (&buff[i]) T(src[i]);
		}
	}
	catch (...)
	{
		_destroy(buff, i);
		throw std::bad_alloc();
	}
	return buff;
}

template <class T>
T* stack<T>::_move(T* src, size_type size, size_type capacity) 
{
	T* buff = static_cast<T*>(std::malloc(sizeof(T) * capacity));
	size_type i;
	try
	{
		for (i = 0; i < size; ++i)
		{
			new (&buff[i]) T(std::move_if_noexcept(src[i]));
		}
	}
	catch (...)
	{
		_destroy(buff, i);
		throw std::bad_alloc();
	}
	return buff;
}

template <class T>
void stack<T>::_destroy(T* data, size_t size)
{
	for (size_t i = size; i > 0; --i)
	{
		data[i - 1].~T();
	}

	std::free(data);
}


