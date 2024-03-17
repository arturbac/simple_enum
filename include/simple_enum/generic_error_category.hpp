// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once
#include <simple_enum/simple_enum.hpp>
#include <simple_enum/basic_fixed_string.hpp>
#include <system_error>
#include <string_view>

namespace simple_enum::inline v0_6
  {
namespace concepts
  {
  /**
   * @concept error_enum
   * @brief Checks if a type is an enum and specialized for std::is_error_code_enum.
   */
  template<typename T>
  concept error_enum = bounded_enum<T> && std::is_error_code_enum<T>::value;

  template<typename T>
  concept error_code_category_name_literal = requires {
    { T::value } -> std::convertible_to<char const *>;
  };
  }  // namespace concepts

template<auto & S>
struct error_code_category_name_literal
  {
  static constexpr auto & value = S;
  };

/**
 * @class generic_error_category
 * @brief A generic error category template for enumerated error codes.
 *
 * This class provides a mechanism to use enumerated types as error codes
 * in a type-safe manner, utilizing the std::error_category interface.
 *
 * @tparam ErrorEnum The enumerated type representing error codes.
 */
template<concepts::error_enum ErrorEnum, concepts::error_code_category_name_literal error_code_category_name>
class generic_error_category : public std::error_category
  {
public:
  /// Returns the name of the error category.
  char const * name() const noexcept override;

  /// Returns the message corresponding to the given error code.
  std::string message(int ev) const override;

  /// Provides access to the singleton instance of the error category.
  static auto instance() -> generic_error_category const &;

  static auto make_error_code(ErrorEnum e) noexcept -> std::error_code;
  };

/**
 * @brief Creates a std::error_code from an enumerated error code.
 *
 * @tparam ErrorEnum The enumerated type representing error codes.
 * @param e The error code of type ErrorEnum.
 * @return std::error_code The corresponding std::error_code object.
 */
template<concepts::error_code_category_name_literal error_code_category_name, concepts::error_enum ErrorEnum>
auto make_error_code(ErrorEnum e) -> std::error_code;

// This part declares that the template specialization of is_error_code_enum is true
// for all types used as ErrorEnum in simple_enum namespace.
/*
namespace std
  {
template<typename ErrorEnum>
struct is_error_code_enum<simple_enum::ErrorEnum> : true_type
  {
  };
  }  // namespace std
*/

/**
 * @example Example usage of generic_error_category with an enumeration
 *
 * .h
 * #include <simple_enum/generic_error_category.hpp>
 * enum class my_error
 *   {
 *   success = 0,
 *   some_other_error,
 *   failure
 *   };
 *
 * // Make my_error bounded enumeration type
 * consteval auto adl_enum_bounds(my_error)
 *   {
 *   using enum my_error;
 *   return simple_enum::adl_info{success, failure};
 *   }
 *
 * //specialize for use as error_code enumeration
 * template<>
 * struct std::is_error_code_enum<my_error> : true_type
 *   {};
 *
 * extern template class simple_enum::generic_error_category<my_error>;
 * extern template std::error_code simple_enum::make_error_code<my_error>(my_error);
 *
 * .cc
 * #include <simple_enum/generic_error_category_impl.hpp>
 * // Explicit instantiation in a corresponding .cpp file
 * template class simple_enum::generic_error_category<my_error>;
 * template std::error_code simple_enum::make_error_code<my_error>(my_error);
 */

  }  // namespace simple_enum::inline v0_6
