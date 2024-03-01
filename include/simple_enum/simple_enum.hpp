#pragma once
#include <concepts>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <string_view>
#include <source_location>
#include <array>

namespace simple_enum::inline v0_1
  {

// clang-format off
template<typename type>
concept enum_concept = std::is_enum_v<type>;

template<typename T>
concept bounded_enum = requires(T e) 
{
  requires enum_concept<T>;
  // Attempt to access 'first' and 'last' assuming they are convertible to T
  { T::first } -> std::convertible_to<T>;
  { T::last } -> std::convertible_to<T>;
  // Additional check to ensure 'last' is greater than or equal to 'first'
  requires (std::to_underlying(T::last) >= std::to_underlying(T::first));
};
// clang-format on
  }  // namespace simple_enum::inline v0_1
//this namespace is for reducing time crunching source location
namespace se
  {
struct mn
  {
  char const * data;
  size_t size;
  };

template<auto enumeration>
constexpr auto se(mn & res, size_t enum_beg ) noexcept -> size_t
  {
  char const * const func{std::source_location::current().function_name()};
  char const * beg{func + enum_beg};
  char const * last_colon{};
  for(; *beg; ++beg)
    if(*beg == ':') [[unlikely]]
      last_colon = beg;

  if(last_colon != nullptr)
    {
    res.data = last_colon + 1;
    res.size = beg - 1 - res.data;
    return size_t(last_colon - func) + 1;
    }
  else
    {
    res = {};
    return 0u;
    }
  }
  }  // namespace se

namespace simple_enum
  {
using meta_name = se::mn;
#if 0
// DEBUG recursion
template<bounded_enum enum_type, std::integral auto first, std::integral auto last, std::integral auto index>
struct dig_enum_members
  {
  consteval static auto dig(std::array<mn_, last - first + 1> & meta) noexcept
    {
    enum_name_meta_constexpr<static_cast<enum_type>(index)>(meta[index - first] );
    dig_enum_members<enum_type, first, last, static_cast<decltype(first)>(index - 1)>::dig(meta);
    }
  };

template<bounded_enum enum_type, std::integral auto first, std::integral auto last>
struct dig_enum_members<enum_type, first, last, first>
  {
  consteval static auto dig(std::array<mn_, last - first + 1> & meta) noexcept
    {
    enum_name_meta_constexpr<static_cast<enum_type>(first)>(meta[first - first] );
    }
  };
#endif
template<typename enum_type, std::integral auto first, std::size_t size, typename name_array, std::size_t... indices>
constexpr void apply_meta_enum(name_array & meta, size_t enum_beg, std::index_sequence<indices...>)
  {
  // Unpack and call enum_name_meta_constexpr for each index, using fold expression
  (..., (se::se<static_cast<enum_type>(first + indices)>(meta[indices], enum_beg)));
  }

template<typename enum_type, std::integral auto first, std::integral auto last, std::size_t size, typename name_array>
constexpr void fold_array(name_array & meta)
  {
  static_assert(size == static_cast<std::size_t>(last - first + 1), "size must match the number of enum values");
  size_t enum_beg{se::se<static_cast<enum_type>(first)>(meta[0], 0u)};
  apply_meta_enum<enum_type, first, size>(meta, enum_beg, std::make_index_sequence<size>{});
  }

template<bounded_enum enum_type>
constexpr auto enum_meta_array() noexcept
  {
  static constexpr auto first_index{std::to_underlying(enum_type::first)};
  static constexpr auto last_index{std::to_underlying(enum_type::last)};
  std::array<meta_name, last_index - first_index + 1> meta;
  // dig_enum_members<enum_type, first_index, last_index, last_index>::dig(meta);
  fold_array<enum_type, first_index, last_index, last_index - first_index + 1>(meta);
  return meta;
  }

template<bounded_enum enum_type>
constexpr auto enum_name(enum_type value) noexcept -> std::string_view
  {
  static constexpr auto first_index{std::to_underlying(enum_type::first)};
  static constexpr auto last_index{std::to_underlying(enum_type::last)};
  static constexpr auto meta{enum_meta_array<enum_type>()};
  auto const requested_index{std::to_underlying(value)};
  if(requested_index >= first_index && requested_index <= last_index)
    {
    meta_name const & res{meta[requested_index-first_index]};
    return std::string_view{res.data, res.size};
    }
  return {};
  }

  }  // namespace simple_enum
