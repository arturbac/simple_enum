// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum

#include <simple_enum/generic_error_category.hpp>
#include <simple_enum/expected.h>
#include <simple_enum/fmtlib_format.hpp>

// >>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>
//
//                                             Header Declaration Section
//
// >>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>

namespace custom_error_example
  {
enum class function_error
  {
  failed_other_reason,
  unhandled_exaption
  };

consteval auto adl_enum_bounds(function_error)
  {
  using enum function_error;
  return simple_enum::adl_info{failed_other_reason, unhandled_exaption};
  }

  }  // namespace custom_error_example

template<>
struct std::is_error_code_enum<custom_error_example::function_error> : true_type
  {
  };

namespace custom_error_example
  {

inline constexpr simple_enum::basic_fixed_string category_name{"Function Error"};
using category_name_literal = simple_enum::string_literal<category_name>;
using function_error_category = simple_enum::generic_error_category<function_error, category_name_literal>;

  }  // namespace custom_error_example

extern template class simple_enum::
  generic_error_category<custom_error_example::function_error, custom_error_example::category_name_literal>;

// >>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>
//
//                                    Embeddable only once in any translation unit
//
// >>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>>>>>-----[[[***]]]-----<<<<<>
#include <simple_enum/generic_error_category_impl.hpp>

using custom_error_example::category_name_literal;
using custom_error_example::function_error;

#ifdef __clang__
#pragma clang diagnostic push
// The design of std::error_code necessitates an instance of error_category, and given that the base class includes a
// virtual destructor, it cannot be declared inline constexpr.
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
template class simple_enum::generic_error_category<function_error, category_name_literal>;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace custom_error_example
  {
using cxx23::expected;
using cxx23::unexpected;

static auto my_function(int arg) -> expected<void, std::error_code>
try
  {
  if(arg != 0)
    return unexpected{function_error_category::make_error_code(function_error::failed_other_reason)};
  return {};
  }
catch(...)
  {
  return unexpected{function_error_category::make_error_code(function_error::unhandled_exaption)};
  }
  }  // namespace custom_error_example

int main(int argc, char const * const *)
  {
  using namespace custom_error_example;
  auto res{my_function(argc)};
  if(!res)
    {
    std::error_code ec{std::move(res.error())};
    fmt::print(
      "Error Category:\"{}\" message:\"{}\" enumeration value:\"{}\"",
      ec.category().name(),
      ec.message(),
      static_cast<function_error>(ec.value())
    );
    }

  return EXIT_SUCCESS;
  }
