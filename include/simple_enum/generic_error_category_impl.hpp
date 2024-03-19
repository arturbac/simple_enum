// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum

#pragma once

#include <simple_enum/generic_error_category.hpp>
#include <simple_enum/basic_fixed_string.hpp>

namespace simple_enum::inline v0_7
  {

template<concepts::error_enum ErrorEnum>
struct meta_enumeration_name
  {
  static constexpr auto error_category_name = simple_enum::enumeration_name_v<ErrorEnum>;
  static constexpr size_t error_category_name_len{error_category_name.size()};
  };

template<concepts::error_enum ErrorEnum>
consteval auto generic_error_category_name()
  {
  static constexpr auto error_category_name = simple_enum::enumeration_name_v<ErrorEnum>;
  static constexpr size_t error_category_name_len{error_category_name.size()};
  return as_basic_fixed_string<char, error_category_name_len>(error_category_name.data());
  }

template<concepts::error_enum ErrorEnum>
char const * generic_error_category<ErrorEnum>::name() const noexcept
  {
  static constexpr auto name_{to_camel_case(generic_error_category_name<ErrorEnum>())};
  return name_.data();
  }

template<concepts::error_enum ErrorEnum>
std::string generic_error_category<ErrorEnum>::message(int ev) const
  {
  return to_camel_case(enum_name(static_cast<ErrorEnum>(ev)));
  }

template<concepts::error_enum ErrorEnum>
auto generic_error_category<ErrorEnum>::instance() -> generic_error_category<ErrorEnum> const &
  {
  static generic_error_category category
#ifdef __clang__
    [[clang::no_destroy]]
#endif
    ;
  return category;
  }

template<concepts::error_enum ErrorEnum>
auto generic_error_category<ErrorEnum>::make_error_code(ErrorEnum e) noexcept -> std::error_code
  {
  return {static_cast<int>(e), generic_error_category<ErrorEnum>::instance()};
  }

template<concepts::error_enum ErrorEnum>
auto make_error_code(ErrorEnum e) -> std::error_code
  {
  return {static_cast<int>(e), generic_error_category<ErrorEnum>::instance()};
  }

  }  // namespace simple_enum::inline v0_7
