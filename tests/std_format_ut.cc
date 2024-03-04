#include <simple_enum/std_format.hpp>
#include "simple_enum_tests.hpp"

int main()
  {
  expect(std::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});

#if defined(__cpp_lib_format) || (defined(__cplusplus) && __cplusplus >= 202002L)
  "std::format formatter test"_test = []
  {
    expect(std::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});
    expect(std::format("{}", lorem_ipsum_short::occaecat) == std::string_view{"occaecat"});
  };
#endif
  }
