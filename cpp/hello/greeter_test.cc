#include "cpp/hello/greeter.h"

#include <gtest/gtest.h>

TEST(GreeterTest, ReturnsExpectedString) {
  EXPECT_EQ(Greet(), "hello from c++, built with bazel");
}
