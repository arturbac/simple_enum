// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once
#include <simple_enum/simple_enum.hpp>

#include <system_error>
#include <string_view>

namespace simple_enum::inline v0_7
  {
namespace concepts
  {
  /**
   * @concept error_enum
   * @brief Checks if a type is an enum and specialized for std::is_error_code_enum.
   */
  template<typename T>
  concept error_enum = bounded_enum<T> && std::is_error_code_enum<T>::value;

  }  // namespace concepts

/**
 * @class generic_error_category
 * @brief A generic error category template for enumerated error codes.
 *
 * This class provides a mechanism to use enumerated types as error codes
 * in a type-safe manner, utilizing the std::error_category interface.
 *
 * @tparam ErrorEnum The enumerated type representing error codes.
 */
template<concepts::error_enum ErrorEnum>
class generic_error_category : public std::error_category
  {
public:
  using enumeration_type = ErrorEnum;

  /// Returns the name of the error category.
  char const * name() const noexcept override;

  /// Returns the message corresponding to the given error code.
  std::string message(int ev) const override;

  static constexpr auto enumeration(std::integral auto ev) noexcept -> enumeration_type
    {
    return static_cast<enumeration_type>(ev);
    }

  /// Provides access to the singleton instance of the error category.
  static auto instance() -> generic_error_category const &;

  /// Creates a std::error_code from an enumerated error code.
  static auto make_error_code(enumeration_type e) noexcept -> std::error_code;
  };

/**
 * @brief Creates a std::error_code from an enumerated error code.
 *
 * @tparam ErrorEnum The enumerated type representing error codes.
 * @param e The error code of type ErrorEnum.
 * @return std::error_code The corresponding std::error_code object.
 */
template<concepts::error_enum ErrorEnum>
auto make_error_code(ErrorEnum e) -> std::error_code;

  }  // namespace simple_enum::inline v0_7
