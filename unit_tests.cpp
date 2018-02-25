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

