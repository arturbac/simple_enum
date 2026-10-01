// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#ifdef SIMPLE_ENUM_CXX_MODULE
import simple_enum;
#else
#include <simple_enum/simple_enum.hpp>
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
#include <fmt/format.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <type_traits>
#include <string_view>

namespace fmt
  {

// Formatter is provided only for bounded enumerations, so it does not hijack enumerations already formattable by
// fmt like std::byte. Format spec is the same as for string_view (fill, align, width, precision). Values without a
// name are formatted as their underlying integer value.
template<typename T>
  requires simple_enum::bounded_enum<T>
struct formatter<T> : formatter<string_view>
  {
  template<typename format_context>
  auto format(T const & e, format_context & ctx) const -> decltype(ctx.out())
    {
    std::string_view const name{simple_enum::enum_name(e)};
    if(!name.empty())
      return formatter<string_view>::format(string_view{name.data(), name.size()}, ctx);

    using underlying_type = std::underlying_type_t<T>;
    using value_type = std::conditional_t<std::is_signed_v<underlying_type>, long long, unsigned long long>;
    format_int const value{static_cast<value_type>(e)};
    return formatter<string_view>::format(string_view{value.data(), value.size()}, ctx);
    }
  };

  }  // namespace fmt

