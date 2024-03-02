#pragma once
#include <concepts>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <string_view>
#include <source_location>
#include <array>

#pragma push_macro("static_constexpr")

#if __cplusplus > 202002L
#define static_constexpr static
#else
#define static_constexpr
#endif

namespace simple_enum::inline v0_1
  {
template<typename Enum>
constexpr auto to_underlying(Enum e) noexcept -> std::underlying_type_t<Enum>
  {
  return static_cast<std::underlying_type_t<Enum>>(e);
  }

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
  requires (to_underlying(T::last) >= to_underlying(T::first));
};
// clang-format on
  }  // namespace simple_enum::inline v0_1

// this namespace is for reducing time crunching source location
namespace se
  {
struct n
  {
  char const * data;
  size_t size;
  };

using s = size_t;
using v = void;
// 0x55581cb6c620 "s se::b(n &) [enumeration = v1]"
// 0x5571fe282666 "s se::b(n &) [enumeration = simple_enum::strong_typed::v1]"
// 0x5571fe2826e2 "s se::b(n &) [enumeration = simple_enum::strong_typed::v2]"
// 0x55c1ac304640 "v se::e(n &, s) [enumeration = v1]"
inline constexpr auto initial_offset{std::char_traits<char>::length("s se::b(n &) [enumeration =")};

template<auto enumeration>
constexpr s b(n & res) noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#else
  char const * const func{std::source_location::current().function_name()};
#endif
  char const * end_of_name{func + initial_offset};
  char const * last_colon{end_of_name};
  for(; *end_of_name; ++end_of_name)
    if(*end_of_name == ':') [[unlikely]]
      last_colon = end_of_name;

  // if(last_colon != nullptr)
  //   {
  res.data = last_colon + 1;
  res.size = size_t(end_of_name - res.data - 1);
  return size_t(last_colon - func) + 1 + 3;  // offset by 3 arg s
  //   }
  // else
  //   {
  //   res = {};
  //   return 0u;
  //   }
  }

template<auto enumeration>
constexpr v e(n & res, s enum_beg) noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#else
  char const * const func{std::source_location::current().function_name()};
#endif
  char const * end_of_name{func + enum_beg};
  char const * enumeration_name{end_of_name};
  while(*end_of_name)
    ++end_of_name;  // for other enumerations we only need to find end of string

  res.data = enumeration_name;
  res.size = size_t(end_of_name - res.data - 1);
  }
  }  // namespace se

namespace simple_enum
  {
using meta_name = se::n;
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
  (..., (se::e<static_cast<enum_type>(first + indices)>(meta[indices + 1], enum_beg)));
  }

template<typename enum_type, std::integral auto first, std::integral auto last, std::size_t size, typename name_array>
constexpr void fold_array(name_array & meta)
  {
  static_assert(size == static_cast<std::size_t>(last - first + 1), "size must match the number of enum values");
  size_t enum_beg{se::b<static_cast<enum_type>(first)>(meta[0])};
  if constexpr(size > 1)
    apply_meta_enum<enum_type, first + 1, size - 1>(meta, enum_beg, std::make_index_sequence<size - 1>{});
  }

template<bounded_enum enum_type>
constexpr auto enum_meta_array() noexcept
  {
  static_constexpr constexpr auto first_index{to_underlying(enum_type::first)};
  static_constexpr constexpr auto last_index{to_underlying(enum_type::last)};
  std::array<meta_name, last_index - first_index + 1> meta;
  // dig_enum_members<enum_type, first_index, last_index, last_index>::dig(meta);
  fold_array<enum_type, first_index, last_index, last_index - first_index + 1>(meta);
  return meta;
  }

template<bounded_enum enum_type>
constexpr auto enum_name(enum_type value) noexcept -> std::string_view
  {
  static_constexpr constexpr auto first_index{to_underlying(enum_type::first)};
  static_constexpr constexpr auto last_index{to_underlying(enum_type::last)};
  static_constexpr constexpr auto meta{enum_meta_array<enum_type>()};
  auto const requested_index{to_underlying(value)};
  if(requested_index >= first_index && requested_index <= last_index)
    {
    meta_name const & res{meta[size_t(requested_index - first_index)]};
    return std::string_view{res.data, res.size};
    }
  return {};
  }
  }  // namespace simple_enum

#pragma pop_macro("static_constexpr")
