// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum

#pragma once

#include <simple_enum/generic_error_category.hpp>

namespace simple_enum::inline v0_6
  {

template<concepts::error_enum ErrorEnum, concepts::error_code_category_name_literal eccn>
char const * generic_error_category<ErrorEnum, eccn>::name() const noexcept
  {
  return eccn::value.data();
  }

template<concepts::error_enum ErrorEnum, concepts::error_code_category_name_literal eccn>
std::string generic_error_category<ErrorEnum, eccn>::message(int ev) const
  {
  // TODO make camel casing const evaluated
  return to_camel_case(enum_name(static_cast<ErrorEnum>(ev)));
  }

template<concepts::error_enum ErrorEnum, concepts::error_code_category_name_literal eccn>
auto generic_error_category<ErrorEnum, eccn>::instance() -> generic_error_category<ErrorEnum, eccn> const &
  {
  static generic_error_category category;
  return category;
  }

template<concepts::error_code_category_name_literal eccn, concepts::error_enum ErrorEnum>
auto make_error_code(ErrorEnum e) -> std::error_code
  {
  return {static_cast<int>(e), generic_error_category<ErrorEnum, eccn>::instance()};
  }

  }  // namespace simple_enum::inline v0_6
