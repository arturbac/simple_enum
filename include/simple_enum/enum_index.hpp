// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#include <simple_enum/simple_enum.hpp>
#include <simple_enum/expected.h>
#include "detail/static_call_operator_prolog.h"

namespace simple_enum::inline v0_9
  {
using cxx23::bad_expected_access;
using cxx23::expected;
using cxx23::in_place;
using cxx23::in_place_t;
using cxx23::unexpect;
using cxx23::unexpect_t;
using cxx23::unexpected;

enum struct enum_index_error
  {
  out_of_range
  };

consteval auto adl_enum_bounds(enum_index_error)
  {
  using enum enum_index_error;
  return simple_enum::adl_info{out_of_range, out_of_range};
  }

struct enum_index_t
  {
  template<enum_concept enum_type>
  [[nodiscard]]
  static_call_operator constexpr auto operator()(enum_type value) static_call_operator_const noexcept
    -> cxx23::expected<std::size_t, enum_index_error>
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    auto const requested_index{simple_enum::detail::to_underlying(value)};
    if(requested_index >= enum_meta_info::first_index() && requested_index <= enum_meta_info::last_index()) [[likely]]
      return requested_index - enum_meta_info::first_index();
    else
      return cxx23::unexpected{enum_index_error::out_of_range};
    }
  };

/**
 * @brief Provides compile or runtime conversion of enum values to their indices.
 *
 * `enum_index_t` is designed to convert enum values to their corresponding indices within the defined range
 *
 * ## Usage
 * The operator() converts an enumeration value to its index, returning a `expected<std::size_t, enum_index_error>`:
 * - On success, it returns the index of the enumeration value.
 * - On failure, it returns `unexpected{enum_index_error::out_of_range}`, indicating the value is outside the valid
 * range.
 *
 * ## Example
 *
 * @code{.cpp}
 * enum class my_enum { value1, value2, value3 };
 *
 * consteval auto adl_enum_bounds(my_enum)
 *  { return simple_enum::adl_info{my_enum::value1, my_enum::value3}; }
 *
 * auto index = enum_index(my_enum::value2);
 * if(index) {
 *     // Use the index
 * } else {
 *     // Handle the error
 * }
 * @endcode
 */
inline constexpr enum_index_t enum_index;

template<enum_concept auto value>
consteval auto consteval_enum_index() -> std::size_t
  {
  return enum_index(value).or_else([](auto &&) { throw; });
  }

/// @brief enumeration value for 0 - based index
template<enum_concept enum_type>
struct emum_index_to_enumeration_t
  {
  [[nodiscard]]
  static_call_operator constexpr auto operator()(std::size_t index) static_call_operator_const noexcept
    -> cxx23::expected<enum_type, enum_index_error>
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    auto const requested_value{enum_meta_info::first_index() + std::underlying_type_t<enum_type>(index)};

    if(requested_value <= enum_meta_info::last_index()) [[likely]]
      return static_cast<enum_type>(requested_value);
    else
      return cxx23::unexpected{enum_index_error::out_of_range};
    }
  };

/// @brief enumeration value for 0 - based index
template<enum_concept enum_type>
inline constexpr emum_index_to_enumeration_t<enum_type> emum_index_to_enumeration;

/// @brief Provides compile time information of length of enumeration (including holes).
template<enum_concept enum_type>
struct enum_size_t
  {
  using enum_meta_info = detail::enum_meta_info_t<enum_type>;
  static constexpr std::size_t value{enum_meta_info::last_index() + 1 - enum_meta_info::first_index()};
  };

template<enum_concept enum_type>
inline constexpr std::size_t enum_size_v = enum_size_t<enum_type>::value;

namespace detail
  {
  [[nodiscard]]
  constexpr bool isdigit(char src) noexcept
    {
    unsigned c{static_cast<unsigned>(src)};
    return c >= 48u && c <= 57u;
    }
  }  // namespace detail

template<enum_concept enum_type>
struct is_valid_enumeration_index_t
  {
  static_call_operator constexpr auto operator()(size_t index) static_call_operator_const noexcept -> bool
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;

    if(index < enum_meta_info::size())
      {
      detail::meta_name const & res{enum_meta_info::meta_data[index]};
      return res.is_valid;
      }
    else
      return false;
    }
  };

template<enum_concept enum_type>
inline constexpr is_valid_enumeration_index_t<enum_type> is_valid_enumeration_index_v;

struct is_valid_enumeration_value_t
  {
  template<enum_concept enum_type>
  static_call_operator constexpr auto operator()(enum_type value) static_call_operator_const noexcept -> bool
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    auto const requested_value{simple_enum::detail::to_underlying(value)};
    if(requested_value >= enum_meta_info::first_index())
      return is_valid_enumeration_index_t<enum_type>{}(size_t(requested_value - enum_meta_info::first_index()));
    else
      return false;
    }
  };

inline constexpr is_valid_enumeration_value_t is_valid_enumeration_value_v;
  }  // namespace simple_enum::inline v0_9

#include "detail/static_call_operator_epilog.h"
