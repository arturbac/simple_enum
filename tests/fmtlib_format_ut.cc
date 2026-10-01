// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#include "simple_enum_tests.hpp"
#include <simple_enum/fmtlib_format.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
#include <fmt/format.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

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

// formatter must not be defined for enumerations without bounds
static_assert(fmt::is_formattable<lorem_ipsum_short>::value);
static_assert(!fmt::is_formattable<format_unbounded_e>::value);

int main()
  {
  "fmtlib formatter test"_test = []
  {
    expect(fmt::format("{}", lorem_ipsum_short::eu) == std::string_view{"eu"});
    expect(fmt::format("{}", lorem_ipsum_short::occaecat) == std::string_view{"occaecat"});
  };
  "fmtlib format spec"_test = []
  {
    expect(eq(fmt::format("{:>4}", E::_2), "  _2"sv));
    expect(eq(fmt::format("{:*<5}|", E::_2), "_2***|"sv));
    expect(eq(fmt::format("{:-^6}", lorem_ipsum_short::eu), "--eu--"sv));
    expect(eq(fmt::format("{:.3}", lorem_ipsum_short::occaecat), "occ"sv));
  };
  "fmtlib value without name"_test = []
  {
    expect(eq(fmt::format("{}", static_cast<E>(200)), "200"sv));
    expect(eq(fmt::format("{}", static_cast<format_signed_e>(-5)), "-5"sv));
    expect(eq(fmt::format("{}", format_signed_e::minus_one), "minus_one"sv));
    expect(eq(fmt::format("{:>5}", static_cast<E>(42)), "   42"sv));
  };
  }
