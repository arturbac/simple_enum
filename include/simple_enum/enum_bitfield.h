// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#include <simple_enum/enum_index.hpp>
#include "detail/static_call_operator_prolog.h"

namespace simple_enum::inline v0_9
  {
template<enum_concept enum_type_t>
struct enum_bitfield_t;

namespace detail
  {
  template<std::size_t enum_count>
  [[nodiscard]]
  consteval auto select_storage_type() noexcept
    {
    static_assert(enum_count > 0, "Enum must have at least one enumerator.");
    static_assert(enum_count <= 64, "Enum has too many members for a uint64_t bitfield.");

    if constexpr(enum_count <= 8)
      return uint8_t{};
    else if constexpr(enum_count <= 16)
      return uint16_t{};
    else if constexpr(enum_count <= 32)
      return uint32_t{};
    else
      return uint64_t{};
    }

  template<enum_concept enum_type_t>
  using select_storage_for_enum_t = decltype(select_storage_type<enum_size_v<enum_type_t>>());

  ///@brief A proxy object to get/set a specific bit in the parent bitfield.
  template<typename parent_type, enum_concept enum_type_t>
  struct bit_proxy_t
    {
    using storage_t = typename enum_bitfield_t<enum_type_t>::storage_t;

    parent_type & parent_;
    enum_type_t value_;

    [[nodiscard]]
    static constexpr auto get_offset(enum_type_t const value) noexcept -> std::size_t
      {
      // enum_size_v/select_storage_for_enum_t would not compile for invalid enum so testing for expected is redudant
      // here
      return *enum_index(value);
      }

    constexpr auto operator=(std::same_as<bool> auto const bit_value) noexcept -> bit_proxy_t &
      {
      auto const offset = get_offset(value_);
      auto const mask = storage_t(1) << offset;

      if(bit_value)
        parent_.bits_ |= mask;
      else
        parent_.bits_ &= ~mask;

      return *this;
      }

    [[nodiscard]]
    constexpr operator bool() const noexcept
      {
      return ((parent_.bits_ >> get_offset(value_)) & 1u) != 0;
      }
    };

  /// \brief compile time bitmsk calculating
  template<std::unsigned_integral T, std::size_t N>
  struct bitmask_t
    {
    static T constexpr value = bitmask_t<T, (N - 1u)>::value | T(T(1u) << (N - 1));
    };

  template<std::unsigned_integral T>
  struct bitmask_t<T, 0u>
    {
    static T constexpr value = T(0u);
    };

  template<std::unsigned_integral T, std::size_t N>
  inline constexpr T bitmask_v = bitmask_t<T, N>::value;
  }  // namespace detail

/// @brief A template struct providing a bitfield with enum indexing.
template<enum_concept enum_type_t>
struct enum_bitfield_t
  {
  using storage_t = detail::select_storage_for_enum_t<enum_type_t>;
  static constexpr storage_t bits_mask{detail::bitmask_v<storage_t, enum_size_v<enum_type_t>>};

  storage_t bits_{0};

  explicit operator storage_t() const noexcept { return bits_; }

  constexpr enum_bitfield_t() noexcept = default;

  explicit constexpr enum_bitfield_t(std::same_as<storage_t> auto bits) noexcept : bits_{bits} {}

  template<std::same_as<enum_type_t>... Args>
  constexpr explicit enum_bitfield_t(Args &&... args) noexcept
    {
    set_values(std::forward<Args>(args)...);
    }

  /**
   * @brief Accesses a bit corresponding to an index of enumeration value.
   * @returns A proxy object to manipulate the bit.
   */
  template<typename Self>
  [[nodiscard]]
  constexpr auto operator[](this Self && self, enum_type_t const value) noexcept
    {
    return detail::bit_proxy_t{self, value};
    }

  template<std::same_as<enum_type_t>... Args>
  constexpr void set_values(enum_type_t const & arg, Args &&... args) noexcept
    {
    detail::bit_proxy_t{*this, arg} = true;
    set_values(std::forward<Args>(args)...);
    }

  constexpr void set_values(enum_type_t const & arg) noexcept { detail::bit_proxy_t{*this, arg} = true; }

  [[nodiscard]]
  constexpr auto operator==(enum_bitfield_t const &) const noexcept -> bool
    = default;

  [[nodiscard]]
  constexpr auto operator~() const noexcept -> enum_bitfield_t
    {
    return enum_bitfield_t{storage_t((~bits_) & bits_mask)};
    }

  [[nodiscard]]
  constexpr auto operator|(enum_bitfield_t const values) const noexcept -> enum_bitfield_t
    {
    return enum_bitfield_t{storage_t(bits_ | values.bits_)};
    }

  [[nodiscard]]
  constexpr auto operator&(enum_bitfield_t const values) const noexcept -> enum_bitfield_t
    {
    return enum_bitfield_t{storage_t(bits_ & values.bits_)};
    }

  [[nodiscard]]
  constexpr auto operator^(enum_bitfield_t const values) const noexcept -> enum_bitfield_t
    {
    return enum_bitfield_t{storage_t((bits_ ^ values.bits_) & bits_mask)};
    }

  constexpr auto operator|=(enum_bitfield_t const values) noexcept -> enum_bitfield_t &
    {
    bits_ |= values.bits_;
    return *this;
    }

  constexpr auto operator&=(enum_bitfield_t const values) noexcept -> enum_bitfield_t &
    {
    bits_ &= values.bits_;
    return *this;
    }

  constexpr auto operator^=(enum_bitfield_t const values) noexcept -> enum_bitfield_t &
    {
    bits_ = (bits_ ^ values.bits_) & bits_mask;
    return *this;
    }
  };

template<enum_concept enum_type_t>
enum_bitfield_t(enum_type_t const & arg) -> enum_bitfield_t<enum_type_t>;

template<enum_concept enum_type_t, typename... Args>
enum_bitfield_t(enum_type_t const & arg, Args &&... args) -> enum_bitfield_t<enum_type_t>;

  }  // namespace simple_enum::inline v0_9

#include "detail/static_call_operator_epilog.h"

