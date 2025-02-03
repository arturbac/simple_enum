// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#ifndef SIMPLE_ENUM_CXX_MODULE
#include <simple_enum/std_format.hpp>
#endif
#include <format>
#include "simple_enum_tests.hpp"
#include <concepts>

int main()
  {
  "std::format formatter test"_test = []
  {
    expect(std::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});
    expect(std::format("{}", lorem_ipsum_short::occaecat) == std::string_view{"occaecat"});
  };
  }
