#include "find_addr.h"

#include <gtest/gtest.h>

TEST(find_addr_test, basic)
{
	std::list<Employee> l;
	l.emplace_back("foo");
	l.emplace_back("bar");

	auto addr = FindAddr(l, "baz");
	EXPECT_TRUE(!addr);

	addr = FindAddr(l, "bar");
	EXPECT_EQ("bar", *addr);
}
