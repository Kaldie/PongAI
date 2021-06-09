#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STREQ("hello", "hello");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}