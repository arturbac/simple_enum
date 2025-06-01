// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#pragma once

#include <simple_enum/enum_index.hpp>
#include <optional>
#include "detail/static_call_operator_prolog.h"

namespace simple_enum::inline v0_9
  {
template<enum_concept enumeration>
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

  template<enum_concept enumeration>
  using select_storage_for_enum_t = decltype(select_storage_type<enum_size_v<enumeration>>());

  constexpr bool test_bit(auto bits, size_t ix) noexcept { return ((bits >> ix) & 1u) != 0; }

  ///@brief A proxy object to get/set a specific bit in the parent bitfield.
  template<typename parent_type, enum_concept enumeration>
  struct bit_proxy_t
    {
    using storage_t = typename enum_bitfield_t<enumeration>::storage_t;

    parent_type & parent_;
    enumeration value_;

    constexpr auto operator=(std::same_as<bool> auto const bit_value) noexcept -> bit_proxy_t &
      {
      auto const offset = *enum_index(value_);
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
      return test_bit(parent_.bits_, *enum_index(value_));
      // return ((parent_.bits_ >> *enum_index(value_)) & 1u) != 0;
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

  template<enum_concept enumeration>
  struct enum_mask_impl_t
    {
    using mask_type = select_storage_for_enum_t<enumeration>;
    static constexpr size_t size{enum_size_v<enumeration>};

    template<size_t N>
    static consteval auto op(mask_type init) noexcept -> mask_type
      {
      if constexpr(N != 0)
        {
        if constexpr(is_valid_enumeration_index_v<enumeration>(N - 1))
          init |= mask_type(mask_type(1u) << (N - 1));
        init |= op<N - 1>(init);
        }
      return init;
      }

    static constexpr mask_type value{op<size>(0)};
    };

  template<enum_concept enumeration>
  inline constexpr auto enum_mask_v = enum_mask_impl_t<enumeration>::value;

  template<enum_concept enum_type_t>
  struct enum_bitfield_traits_t
    {
    using storage_t = detail::select_storage_for_enum_t<enum_type_t>;
    // mask with eanbled bits of all valid mapped enumerations indexes, it does not enable indexes for holes in enum
    static constexpr storage_t bits_mask{detail::enum_mask_v<enum_type_t>};
    };

  template<enum_concept enumeration>
  struct enum_bitfield_iterator_t
    {
    using traits_type = detail::enum_bitfield_traits_t<enumeration>;
    using storage_t = traits_type::storage_t;
    static constexpr storage_t bits_mask{traits_type::bits_mask};

    using iterator_category = std::forward_iterator_tag;
    using value_type = enumeration;
    using difference_type = std::ptrdiff_t;
    using pointer = enumeration const *;
    using reference = enumeration const &;

    storage_t bits_{};
    std::optional<enumeration> current_{};

    constexpr enum_bitfield_iterator_t() noexcept = default;

    constexpr explicit enum_bitfield_iterator_t(storage_t bits) noexcept : bits_{bits}, current_{}
      {
      // move to first
      static constexpr size_t size{enum_size_v<enumeration>};
      for(size_t ix{}; ix != size; ++ix)
        if(test_bit(bits_, ix))  // no need to test if it is valid as mask will always maintain that
          {
          current_ = *emum_index_to_enumeration<enumeration>(ix);
          break;
          }
      }

    constexpr auto operator*() const noexcept -> reference { return *current_; }

    constexpr auto operator->() const noexcept -> pointer { return current_->value(); }

    constexpr auto operator++() noexcept -> enum_bitfield_iterator_t &
      {
      if(current_)
        {
        static constexpr size_t size{enum_size_v<enumeration>};
        size_t ix{*enum_index(*current_)};
        current_.reset();
        for(++ix; ix != size; ++ix)
          if(test_bit(bits_, ix))  // no need to test if it is valid as mask will always maintain that
            {
            current_ = *emum_index_to_enumeration<enumeration>(ix);
            break;
            }
        }
      return *this;
      }

    constexpr auto operator++(int) noexcept -> enum_bitfield_iterator_t
      {
      enum_bitfield_iterator_t tmp{*this};
      ++(*this);
      return tmp;
      }

    constexpr auto operator==(enum_bitfield_iterator_t const & other) const noexcept -> bool = default;
    };
  }  // namespace detail

enum struct enum_bitfield_full_e
  {
  };

/// @brief A template struct providing a bitfield with enum indexing.
template<enum_concept enumeration>
struct enum_bitfield_t
  {
  using traits_type = detail::enum_bitfield_traits_t<enumeration>;
  using storage_t = traits_type::storage_t;
  static constexpr storage_t bits_mask{traits_type::bits_mask};
  using iterator = detail::enum_bitfield_iterator_t<enumeration>;

  struct sentinel_t
    {
    constexpr auto operator==(iterator const & it) const noexcept { return not it.current_.has_value(); }
    };

  storage_t bits_{0};

  explicit operator storage_t() const noexcept { return bits_; }

  constexpr enum_bitfield_t() noexcept = default;

  explicit constexpr enum_bitfield_t(std::same_as<storage_t> auto bits) noexcept : bits_{bits} {}

  // constructs will full bits enabled
  explicit constexpr enum_bitfield_t(enum_bitfield_full_e) noexcept : bits_{bits_mask} {}

  template<std::same_as<enumeration>... Args>
  constexpr explicit enum_bitfield_t(Args &&... args) noexcept
    {
    set_values(std::forward<Args>(args)...);
    }

  constexpr auto begin() const noexcept -> iterator { return iterator{bits_}; }

  constexpr auto end() const noexcept -> sentinel_t { return sentinel_t{}; }

  /**
   * @brief Accesses a bit corresponding to an index of enumeration value.
   * @returns A proxy object to manipulate the bit.
   */
  template<typename Self>
  [[nodiscard]]
  constexpr auto operator[](this Self && self, enumeration const value) noexcept
    {
    return detail::bit_proxy_t{self, value};
    }

  template<std::same_as<enumeration>... Args>
  constexpr void set_values(enumeration const & arg, Args &&... args) noexcept
    {
    detail::bit_proxy_t{*this, arg} = true;
    set_values(std::forward<Args>(args)...);
    }

  constexpr void set_values(enumeration const & arg) noexcept { detail::bit_proxy_t{*this, arg} = true; }

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

template<enum_concept enumeration>
enum_bitfield_t(enumeration const & arg) -> enum_bitfield_t<enumeration>;

template<enum_concept enumeration, typename... Args>
enum_bitfield_t(enumeration const & arg, Args &&... args) -> enum_bitfield_t<enumeration>;

  }  // namespace simple_enum::inline v0_9

#include "detail/static_call_operator_epilog.h"

