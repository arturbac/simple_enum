// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#include <iostream>
#include <cstddef>      // for std::size_t
#include <type_traits>  // for std::is_same
#include <simple_enum/detail/stralgo_inplace.hpp>
#include <algorithm>

namespace simple_enum::inline v0_6
  {
namespace concepts
  {
  template<typename T>
  concept string_literal = requires {
    { T::value } -> std::convertible_to<char const *>;
  };

  }  // namespace concepts

template<typename CharT, std::size_t N>
struct basic_fixed_string
  {
  CharT str[N + 1]{};

  constexpr operator CharT const *() const noexcept { return str; }

  constexpr CharT const * data() const noexcept { return str; }

  constexpr auto view() const noexcept { return std::basic_string_view{data(), size()}; }

  constexpr operator std::basic_string_view<CharT>() const noexcept { return view(); }

  static constexpr std::size_t size() noexcept { return N; }

  constexpr auto operator==(basic_fixed_string const & rh) const noexcept -> bool = default;
  };

template<typename T, std::size_t N>
basic_fixed_string(T const (&)[N]) -> basic_fixed_string<T, N - 1>;

template<auto & S>
struct string_literal
  {
  static constexpr auto & value = S;
  };

template<typename CharT, std::size_t N>
consteval auto to_camel_case(basic_fixed_string<CharT, N> const & input) -> basic_fixed_string<CharT, N>
  {
  basic_fixed_string<CharT, N> result{};
  bool to_upper = true;

  for(std::size_t i = 0; i < N; ++i)
    {
    if(input.str[i] == '_' || input.str[i] == ' ' || input.str[i] == '\0')
      {
      to_upper = true;
      if(input.str[i] == '_')
        result.str[i] = ' ';  // Copy space or null character as is
      else
        result.str[i] = input.str[i];  // Copy space or null character as is
      }
    else if(to_upper)
      {
      result.str[i] = detail::to_upper(input.str[i]);
      to_upper = false;
      }
    else
      {
      result.str[i] = input.str[i];
      }
    }

  return result;
  }

template<typename CharT>
constexpr auto to_camel_case(std::basic_string_view<CharT> input) -> std::basic_string<CharT>
  {
  std::basic_string<CharT> result{};
  bool to_upper = true;
  result.reserve(input.size());
  std::ranges::transform(
    input,
    std::back_inserter(result),
    [&to_upper](CharT c) noexcept -> CharT
    {
      if(c == '_' || c == ' ' || c == '\0')
        {
        to_upper = true;
        if(c == '_')
          return ' ';
        else
          return c;  // Copy space or null character as is
        }
      else if(to_upper)
        {
        to_upper = false;
        return detail::to_upper(c);
        }
      else
        return c;
    }
  );

  return result;
  }
  }  // namespace simple_enum::inline v0_6
