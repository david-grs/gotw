#include "stack.h"

#include <gtest/gtest.h>

class stack_test : public ::testing::Test
{

protected:
	stack<int> _s;
};

TEST_F(stack_test, init)
{
	EXPECT_TRUE(_s.empty());
	EXPECT_EQ(0u, _s.size());
}

TEST_F(stack_test, push_once)
{
	_s.push(1);
	EXPECT_EQ(1u, _s.size());
}

TEST_F(stack_test, push_multiples)
{
	_s.push(1);
	_s.push(2);
	_s.push(3);
	_s.push(4);
	EXPECT_EQ(4u, _s.size());
	EXPECT_EQ(4, _s.pop());
}

TEST_F(stack_test, push_many)
{
	static const int ElementsCount = 100000;
	for (int i = 0; i < ElementsCount; ++i)
	{
		_s.push(i);
	}

	EXPECT_EQ(ElementsCount, _s.size());
	for (std::size_t i = 0; i < ElementsCount; ++i)
	{
		EXPECT_EQ(ElementsCount - i - 1, _s.pop());
	}
	EXPECT_EQ(0, _s.size());
	EXPECT_TRUE(_s.empty());
}

TEST_F(stack_test, pop)
{
	_s.push(1);
	EXPECT_EQ(1, _s.pop());
	EXPECT_TRUE(_s.empty());

	_s.push(1);
	_s.push(2);
	_s.push(3);
	EXPECT_EQ(3, _s.pop());
	EXPECT_FALSE(_s.empty());
	EXPECT_EQ(2, _s.size());
}

TEST_F(stack_test, pop_throw)
{
	EXPECT_THROW(_s.pop(), std::runtime_error);
}

TEST_F(stack_test, copy_ctor_empty)
{
	auto s = _s;
	EXPECT_TRUE(s.empty());
}

class copy_stack_test : public stack_test
{
public:
	copy_stack_test()
	{
		_s.push(1);
		_s.push(2);
		_s.push(3);
	}

	void consume(stack<int>& ss)
	{
		EXPECT_EQ(3u, ss.size());
		EXPECT_EQ(3, ss.pop());
		EXPECT_EQ(2, ss.pop());
		EXPECT_EQ(1, ss.pop());
		EXPECT_TRUE(ss.empty());
	}
};

TEST_F(copy_stack_test, copy_ctor)
{
	auto s = _s;

	consume(s);
	consume(_s);
}

TEST_F(copy_stack_test, assignment)
{
	stack<int> s;
	s.push(0);
	EXPECT_EQ(1u, s.size());

	s = _s;

	consume(s);
	consume(_s);
}

class UnsafeToCopy
{
public:
	UnsafeToCopy(int i) :
		_i(i)
	{}

	UnsafeToCopy(const UnsafeToCopy& u) :
		_i(u._i)
	{
		if (u._i == 42)
			throw std::runtime_error("foo");
	}

	int get() const { return _i; }

private:
	int _i;
};

class stack_exception_test : public ::testing::Test
{
public:
	stack_exception_test()
	{
		_s.emplace(42);
		_s.emplace(1);
	}

	void TearDown() override
	{
		EXPECT_EQ(2u, _s.size());
		EXPECT_EQ(1, _s.pop().get());
		EXPECT_THROW(_s.pop(), std::runtime_error);
	}

protected:
	stack<UnsafeToCopy> _s;
};

TEST_F(stack_exception_test, copy_ctor_throw)
{
	EXPECT_THROW(auto s = _s, std::runtime_error);
}

TEST_F(stack_exception_test, assignment_throw)
{
	stack<UnsafeToCopy> s;
	s.emplace(1);
	s.emplace(2);
	s.emplace(3);

	EXPECT_THROW(s = _s, std::runtime_error);

	EXPECT_EQ(3u, s.size());
	EXPECT_EQ(3, s.pop().get());
	EXPECT_EQ(2, s.pop().get());
	EXPECT_EQ(1, s.pop().get());
}

class counter
{
public:
	explicit counter(const std::string& t) : _t(t) {}

	counter(const counter& c) : _t(c._t) { ++copies; }
	counter& operator=(const counter& c) { _t = c._t; ++copies; return *this; }

	counter(counter&& c) noexcept : _t(std::move(c._t)) { ++moves; }
	counter& operator=(const counter&& c) noexcept { _t = std::move(c._t); ++moves; return *this; }

	const std::string & get() const { return _t; }

	static void reset() { copies = 0; moves = 0; }

	static int moves;
	static int copies;

private:
	std::string _t;
};

int counter::copies = 0;
int counter::moves = 0;



class stack_moves_test : public ::testing::Test
{
public:

protected:
	stack<counter> _s;
};

TEST_F(stack_moves_test, reserve)
{
	_s.emplace("foo");
	_s.emplace("bar");

	counter::reset();

	_s.emplace("buz");
	EXPECT_EQ(0, counter::copies);
	EXPECT_EQ(2, counter::moves);
}

TEST_F(stack_moves_test, copies)
{
	_s.emplace("foo");
	_s.emplace("bar");

	auto s = _s;
	EXPECT_EQ("bar", s.pop().get());
	EXPECT_EQ("foo", s.pop().get());

	EXPECT_EQ("bar", _s.pop().get());
	EXPECT_EQ("foo", _s.pop().get());
}

TEST_F(stack_moves_test, moves)
{
	_s.emplace("foo");
	_s.emplace("bar");

	auto s = std::move(_s);

	EXPECT_EQ("bar", s.pop().get());
	EXPECT_EQ("foo", s.pop().get());
}





