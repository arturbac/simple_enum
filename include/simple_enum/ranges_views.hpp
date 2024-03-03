#pragma once

#include <simple_enum/simple_enum.hpp>
#include <iterator>
#include <ranges>

namespace simple_enum::inline v0_5::detail
  {
template<bounded_enum enumeration>
class enum_view_iterator
  {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = enumeration;
  using difference_type = std::ptrdiff_t;
  using pointer = enumeration const *;
  using reference = enumeration const &;

private:
  enumeration current_{};

public:
  constexpr enum_view_iterator() noexcept = default;

  constexpr explicit enum_view_iterator(enumeration current) : current_(current) {}

  constexpr auto operator*() const noexcept -> reference { return current_; }

  constexpr auto operator->() const noexcept -> pointer { return &current_; }

  constexpr auto operator++() noexcept -> enum_view_iterator &
    {
    current_ = static_cast<enumeration>(simple_enum::to_underlying(current_) + 1);
    return *this;
    }

  constexpr auto operator++(int) noexcept -> enum_view_iterator
    {
    enum_view_iterator tmp = *this;
    ++(*this);
    return tmp;
    }

  constexpr auto operator--() noexcept -> enum_view_iterator &
    {
    current_ = static_cast<enumeration>(simple_enum::to_underlying(current_) - 1);
    return *this;
    }

  constexpr auto operator--(int) noexcept -> enum_view_iterator
    {
    enum_view_iterator tmp = *this;
    --(*this);
    return tmp;
    }

  constexpr auto operator+=(difference_type n) noexcept -> enum_view_iterator &
    {
    current_ = static_cast<enumeration>(simple_enum::to_underlying(current_) + n);
    return *this;
    }

  constexpr auto operator-=(difference_type n) noexcept -> enum_view_iterator &
    {
    current_ = static_cast<enumeration>(simple_enum::to_underlying(current_) - n);
    return *this;
    }

  constexpr auto operator-(enum_view_iterator const & other) const noexcept -> difference_type
    {
    return simple_enum::to_underlying(current_) - simple_enum::to_underlying(other.current_);
    }

  constexpr auto operator[](difference_type n) const noexcept -> reference { return *(*this + n); }

  constexpr auto operator<(enum_view_iterator const & other) const noexcept -> bool
    {
    return simple_enum::to_underlying(current_) < simple_enum::to_underlying(other.current_);
    }

  constexpr auto operator>(enum_view_iterator const & other) const noexcept -> bool
    {
    return simple_enum::to_underlying(current_) > simple_enum::to_underlying(other.current_);
    }

  constexpr auto operator<=(enum_view_iterator const & other) const noexcept -> bool
    {
    return simple_enum::to_underlying(current_) <= simple_enum::to_underlying(other.current_);
    }

  constexpr auto operator>=(enum_view_iterator const & other) const noexcept -> bool
    {
    return simple_enum::to_underlying(current_) >= simple_enum::to_underlying(other.current_);
    }

  constexpr auto operator==(enum_view_iterator const & other) const noexcept -> bool = default;
  };
  }  // namespace simple_enum::inline v0_5::detail

namespace simple_enum::inline v0_5
  {

template<bounded_enum enumeration>
class enum_view : public std::ranges::view_interface<enum_view<enumeration>>
  {
  static_assert(std::is_enum_v<enumeration>, "enumeration must be an enum type");

public:
  using iterator = detail::enum_view_iterator<enumeration>;

private:
  enumeration first_;
  enumeration last_;

public:
  constexpr enum_view() noexcept : first_(detail::bounds<enumeration>::first), last_(detail::bounds<enumeration>::last)
    {
    }

  constexpr enum_view(enumeration first, enumeration last) noexcept : first_(first), last_(last) {}

  constexpr auto begin() const noexcept -> iterator { return iterator(first_); }

  constexpr auto end() const noexcept -> iterator
    {
    return iterator(static_cast<enumeration>(simple_enum::to_underlying(last_) + 1));
    }
  };

template<typename enumeration>
enum_view(enumeration) -> enum_view<enumeration>;

template<typename enumeration>
enum_view(enumeration, enumeration) -> enum_view<enumeration>;

template<typename enumeration>
constexpr auto begin(enum_view<enumeration> const & v) -> typename enum_view<enumeration>::iterator
  {
  return v.begin();
  }

template<typename enumeration>
constexpr auto end(enum_view<enumeration> const & v) -> typename enum_view<enumeration>::iterator
  {
  return v.end();
  }
  }  // namespace simple_enum::inline v0_5

namespace std::ranges
  {

template<typename enumeration>
inline constexpr bool enable_borrowed_range<simple_enum::enum_view<enumeration>> = true;

  }  // namespace std::ranges
