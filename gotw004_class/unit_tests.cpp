#include "complex.h"

#include <gtest/gtest.h>

TEST(complex_test, basic)
{
	Complex c(1, 2);
	Complex c2(0, 1);

	++c;
	EXPECT_DOUBLE_EQ(2.0, c.real());
	EXPECT_DOUBLE_EQ(3.0, (c + c2).imaginary());
}
