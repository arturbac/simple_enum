// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#include <simple_enum/std_format.hpp>
#include "simple_enum_tests.hpp"
#include <concepts>

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
