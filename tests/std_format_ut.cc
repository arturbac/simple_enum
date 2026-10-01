// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#ifndef SIMPLE_ENUM_CXX_MODULE
#include <simple_enum/std_format.hpp>
#endif
#include <format>
#include "simple_enum_tests.hpp"
#include <concepts>
#include <cstddef>
#include <system_error>

enum struct format_signed_e : int8_t
  {
  minus_one = -1,
  zero,
  first = minus_one,
  last = zero
  };

enum struct format_unbounded_e
  {
  v0,
  v1
  };

#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
// formatter must not be defined for enumerations without bounds, especially std:: ones
static_assert(std::formattable<lorem_ipsum_short, char>);
static_assert(!std::formattable<format_unbounded_e, char>);
static_assert(!std::formattable<std::byte, char>);
static_assert(!std::formattable<std::errc, char>);
#endif

int main()
  {
  "std::format formatter test"_test = []
  {
    expect(std::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});
    expect(std::format("{}", lorem_ipsum_short::occaecat) == std::string_view{"occaecat"});
  };
  "std::format format spec"_test = []
  {
    expect(eq(std::format("{:>4}", E::_2), "  _2"sv));
    expect(eq(std::format("{:*<5}|", E::_2), "_2***|"sv));
    expect(eq(std::format("{:-^6}", lorem_ipsum_short::eu), "--eu--"sv));
    expect(eq(std::format("{:.3}", lorem_ipsum_short::occaecat), "occ"sv));
  };
  "std::format value without name"_test = []
  {
    expect(eq(std::format("{}", static_cast<E>(200)), "200"sv));
    expect(eq(std::format("{}", static_cast<format_signed_e>(-5)), "-5"sv));
    expect(eq(std::format("{}", format_signed_e::minus_one), "minus_one"sv));
    expect(eq(std::format("{:>5}", static_cast<E>(42)), "   42"sv));
  };
  }
