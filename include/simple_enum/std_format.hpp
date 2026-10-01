// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#include <simple_enum/simple_enum.hpp>
#include <type_traits>
#include <format>
#include <charconv>
#include <array>
#include <memory>
#include <string_view>

// Formatter is provided only for bounded enumerations, so it does not hijack std:: enumerations like std::byte or
// std::errc. Format spec is the same as for std::string_view (fill, align, width, precision). Values without a name
// are formatted as their underlying integer value.
template<simple_enum::bounded_enum enumeration>
struct std::formatter<enumeration> : std::formatter<std::string_view>
  {
  template<typename format_context>
  auto format(enumeration const & e, format_context & ctx) const -> decltype(ctx.out())
    {
    std::string_view const name{simple_enum::enum_name(e)};
    if(!name.empty())
      return std::formatter<std::string_view>::format(name, ctx);

    using underlying_type = std::underlying_type_t<enumeration>;
    using value_type = std::conditional_t<std::is_signed_v<underlying_type>, long long, unsigned long long>;
    std::array<char, 24> buffer;
    auto const res{std::to_chars(buffer.data(), std::to_address(buffer.end()), static_cast<value_type>(e))};
    return std::formatter<std::string_view>::format(std::string_view{buffer.data(), res.ptr}, ctx);
    }
  };
