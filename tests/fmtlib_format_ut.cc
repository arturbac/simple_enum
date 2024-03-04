#include <simple_enum/fmtlib_format.hpp>
#include "simple_enum_tests.hpp"
#include <fmt/format.h>

int main()
  {
  "fmtlib formatter test"_test = []
  {
    expect(fmt::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});
    expect(fmt::format("{}", lorem_ipsum_short::occaecat) == std::string_view{"occaecat"});
    // Add more tests for other enum values as needed
  };
  }
