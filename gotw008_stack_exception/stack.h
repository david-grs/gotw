#pragma once

#include <type_traits>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <stdexcept>

#include <memory>

template <class T>
class stack_base
{
public:
	using value_type = T;
	using size_type = std::size_t;

	explicit stack_base() noexcept :
		_data(nullptr),
		_capacity(0),
		_size(0)
	{ }

	explicit stack_base(size_type capacity) :
		_data(static_cast<T*>(capacity == 0 ? nullptr : operator new(sizeof(T) * capacity))),
		_capacity(capacity),
		_size(0)
	{ }

	~stack_base() noexcept
	{
		if (_data)
		{
			std::destroy(_data, _data + _size);
			operator delete(_data);
			_data = nullptr;
		}
	}

	stack_base(const stack_base&) =delete;
	stack_base(stack_base&&) =delete;

	template <class... Args>
	void construct_at(size_type pos, Args&&... args)
	{
		assert(pos < _capacity);
		new (&_data[pos]) T(std::forward<Args>(args)...);
	}

protected:
	void swap(stack_base<T>&);

	T* _data = nullptr;
	size_type _capacity = 0;
	size_type _size = 0;
};

template <class T>
void stack_base<T>::swap(stack_base<T>& other)
{
	using std::swap;
	swap(_data, other._data);
	swap(_size, other._size);
	swap(_capacity, other._capacity);
}

template <class T>
class stack : private stack_base<T>
{
	using stack_base<T>::_size;
	using stack_base<T>::_capacity;
	using stack_base<T>::_data;
	using stack_base<T>::swap;
	using stack_base<T>::construct_at;

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
	stack(size_type capacity);
};

template <class T>
stack<T>::stack() noexcept
{}

template <class T>
stack<T>::stack(size_type capacity) :
	stack_base<T>(capacity)
{}

template <class T>
stack<T>::~stack() noexcept
{}

template <class T>
stack<T>::stack(const stack& other)
noexcept(std::is_nothrow_copy_constructible<T>::value) :
	stack_base<T>(other._capacity)
{
	for (size_type pos = 0; pos < other._size; ++pos)
	{
		construct_at(pos, other._data[pos]);
		++_size;
	}
}

template <class T>
stack<T>::stack(stack&& other)
noexcept(std::is_nothrow_move_constructible<T>::value) :
	stack_base<T>(other._capacity)
{
	for (size_type pos = 0; pos < other._size; ++pos)
	{
		construct_at(pos, std::move_if_noexcept(other._data[pos]));
		++_size;
	}
}

template <class T>
stack<T>& stack<T>::operator=(stack<T> s)
noexcept(std::is_nothrow_copy_constructible<T>::value)
{
	swap(s);
	return *this;
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

	construct_at(_size, std::forward<Args>(args)...);
	++_size;
}

template <class T>
void stack<T>::reserve(size_type new_capacity)
{
	assert(new_capacity >= _capacity);

	stack<T> new_stack(new_capacity);
	for (size_type pos = 0; pos < _size; ++pos)
	{
		new_stack.construct_at(pos, std::move_if_noexcept(_data[pos]));
		++new_stack._size;
	}

	swap(new_stack);
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

