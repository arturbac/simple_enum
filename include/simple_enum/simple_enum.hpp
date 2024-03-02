#pragma once

#include <concepts>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <string_view>
#include <source_location>
#include <array>

#define SIMPLE_ENUM_NAME_VERSION "0.2.2"

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
concept lower_bounded_enum = requires(T e) 
{
  requires enum_concept<T>;
  { T::first } -> std::convertible_to<T>;
};
template<typename T>
concept upper_bounded_enum = requires(T e) 
{
  requires enum_concept<T>;
  { T::last } -> std::convertible_to<T>;
};
template<typename T>
concept bounded_enum = requires(T e) 
{
  requires enum_concept<T>;
  requires lower_bounded_enum<T>;
  requires upper_bounded_enum<T>;
  requires (to_underlying(T::last) >= to_underlying(T::first));
};
// clang-format on

#ifndef SIMPLE_ENUM_CUSTOM_UNBOUNDED_RANGE
#define SIMPLE_ENUM_CUSTOM_UNBOUNDED_RANGE
inline constexpr auto default_unbounded_uuper_range = 10;
#endif
// enum struct enum_meta_bounded { no, yes };

struct bounds_info
  {
  bool lower_bound;
  bool upper_bound;
  };

///\brief info class is intended to custom specialize by users if they are unable to modify enum and wish to add bound
/// info
template<typename enumeration, bounds_info = bounds_info{}>
struct info
  {
  static constexpr enumeration first = static_cast<enumeration>(0);
  static constexpr enumeration last = static_cast<enumeration>(default_unbounded_uuper_range);
  };

template<typename enumeration>
struct info<enumeration, bounds_info{.lower_bound = true, .upper_bound = true}>
  {
  static constexpr enumeration first = enumeration::first;
  static constexpr enumeration last = enumeration::last;
  };

// default assume range 0 - 10
template<typename enumeration>
struct info<enumeration, bounds_info{.lower_bound = true, .upper_bound = false}>
  {
  static constexpr enumeration first = enumeration::first;
  static constexpr enumeration last = static_cast<enumeration>(default_unbounded_uuper_range);
  };

template<typename enumeration>
struct info<enumeration, bounds_info{.lower_bound = false, .upper_bound = true}>
  {
  static constexpr enumeration first = static_cast<enumeration>(0);
  static constexpr enumeration last = enumeration::last;
  };

namespace detail
  {
  template<typename enumeration>
  struct bounds
    {
    static constexpr bounds_info
      bi_{.lower_bound = lower_bounded_enum<enumeration>, .upper_bound = upper_bounded_enum<enumeration>};
    using info_t = info<enumeration, bi_>;
    static constexpr enumeration first = info_t::first;
    static constexpr enumeration last = info_t::last;
    static constexpr auto first_index = to_underlying(info_t::first);
    static constexpr auto last_index = to_underlying(info_t::last);
    };
  }  // namespace detail
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

template<auto enumeration>
constexpr auto f() noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#else
  char const * const func{std::source_location::current().function_name()};
#endif
  return func;
  }

#if defined(__clang__)
#if __clang_major__ < 13
#error "Clang minimal supported version is 13"
#endif
// This is just for speeding up user use cases to skip computation of strlen at compile time

// index of the = character in the given line "auto se::f() [enumeration =" is 26
inline constexpr auto initial_offset{26 + 1};
inline constexpr auto end_of_enumeration_name = ']';

#elif defined(__GNUC__)
// index of the = character in the given line "constexpr auto se::f() [with auto enumeration =" is 46.
inline constexpr auto initial_offset{46 + 1};
inline constexpr char end_of_enumeration_name = ']';

#elif defined(_MSC_VER)
#error "msvc support is under development"
#else
#error "supply information to author about Your compiler"
#endif

#ifdef SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS
// OPT IN TESTING CODE
enum struct verify_ennum_
  {
  v1
  };

constexpr size_t find_enumeration_offset()
  {
  auto const func{std::string_view{f<verify_ennum_::v1>()}};
  size_t pos = func.find("enumeration =");
  if(pos == std::string_view::npos)
    throw;
  return pos + 12 + 1;
  }

auto constexpr verify_offset() -> bool { return find_enumeration_offset() == initial_offset; }

static_assert(verify_offset());
#endif

template<auto enumeration>
constexpr auto b(n & res) noexcept
  {
  char const * const func{f<enumeration>()};
  char const * end_of_name{func + initial_offset};
  char const * last_colon{end_of_name};
  for(; *end_of_name; ++end_of_name)
    if(*end_of_name == ':' || *end_of_name == ')') [[unlikely]]
      last_colon = end_of_name;

  res.data = last_colon + 1;
  res.size = size_t(end_of_name - res.data - 1);
  return size_t(last_colon - func) + 1;
  }

template<auto enumeration>
constexpr void e(n & res, s enum_beg) noexcept
  {
  char const * const func{f<enumeration>()};
  char const * end_of_name{func + enum_beg};
  char const * enumeration_name{end_of_name};
  while(*end_of_name != end_of_enumeration_name)
    ++end_of_name;  // for other enumerations we only need to find end of string

  res.data = enumeration_name;
  res.size = size_t(end_of_name - res.data);
  }
  }  // namespace se

namespace simple_enum
  {
using meta_name = se::n;

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

template<enum_concept enum_type>
constexpr auto enum_meta_array() noexcept
  {
  using bounds_type = detail::bounds<enum_type>;
  static_constexpr constexpr auto first_index{bounds_type::first_index};
  static_constexpr constexpr auto last_index{bounds_type::last_index};
  // static_constexpr constexpr auto first_index{to_underlying(enum_type::first)};
  // static_constexpr constexpr auto last_index{to_underlying(enum_type::last)};

  std::array<meta_name, last_index - first_index + 1> meta;
  // dig_enum_members<enum_type, first_index, last_index, last_index>::dig(meta);
  fold_array<enum_type, first_index, last_index, last_index - first_index + 1>(meta);
  return meta;
  }

template<enum_concept enum_type>
constexpr auto enum_name(enum_type value) noexcept -> std::string_view
  {
  using bounds_type = detail::bounds<enum_type>;
  static_constexpr constexpr auto first_index{bounds_type::first_index};
  static_constexpr constexpr auto last_index{bounds_type::last_index};
  // static_constexpr constexpr auto first_index{to_underlying(enum_type::first)};
  // static_constexpr constexpr auto last_index{to_underlying(enum_type::last)};
  static_constexpr constexpr auto meta{enum_meta_array<enum_type>()};
  auto const requested_index{to_underlying(value)};
  if(requested_index >= first_index && requested_index <= last_index)
    {
    meta_name const & res{meta[size_t(requested_index - first_index)]};
    return std::string_view{res.data, res.size};
    }
  else
    return {""};  // return empty but null terminated
  }
  }  // namespace simple_enum

#pragma pop_macro("static_constexpr")
