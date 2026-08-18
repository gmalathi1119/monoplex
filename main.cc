// Minimal binary used to validate the C++ toolchain wiring in Bazel.
#include <iostream>

#include "cpp/hello/greeter.h"

int main() {
  std::cout << Greet() << std::endl;
  return 0;
}
