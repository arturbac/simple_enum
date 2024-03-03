#pragma once

#include <concepts>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <string_view>
#if defined(__cpp_lib_source_location)
#include <source_location>
#endif
#include <array>

#define SIMPLE_ENUM_NAME_VERSION "0.5.0"

#pragma push_macro("static_constexpr")
#pragma push_macro("static_call_operator")
#pragma push_macro("static_call_operator_const")

#if __cplusplus > 202002L
#define static_constexpr static
#else
#define static_constexpr
#endif

#if __cplusplus >= 202301L && defined(__cpp_static_call_operator)
#define static_call_operator static
#define static_call_operator_const
#else
#define static_call_operator
#define static_call_operator_const const
#endif

namespace simple_enum::inline v0_4
  {
template<typename Enum>
constexpr auto to_underlying(Enum e) noexcept -> std::underlying_type_t<Enum>
  {
  return static_cast<std::underlying_type_t<Enum>>(e);
  }

// clang-format off
template<typename type>
concept enum_concept = std::is_enum_v<type>;

// clang-format on

#ifndef SIMPLE_ENUM_CUSTOM_UNBOUNDED_RANGE
#define SIMPLE_ENUM_CUSTOM_UNBOUNDED_RANGE
inline constexpr auto default_unbounded_upper_range = 10;
#endif

///\brief info class is intended to custom specialize by users if they are unable to modify enum and wish to add bound
/// info
template<typename enumeration>
struct info
  {
  };

namespace detail
  {

  template<typename T>
  concept lower_bounded_enum = requires(T e) {
    requires enum_concept<T>;
      {
      T::first
      } -> std::convertible_to<T>;
  };
  template<typename T>
  concept upper_bounded_enum = requires(T e) {
    requires enum_concept<T>;
      {
      T::last
      } -> std::convertible_to<T>;
  };
  template<typename T>
  concept internaly_bounded_enum = requires(T e) {
    requires enum_concept<T>;
    requires lower_bounded_enum<T>;
    requires upper_bounded_enum<T>;
    requires(simple_enum::to_underlying(T::last) >= simple_enum::to_underlying(T::first));
  };

  template<typename enumeration, typename = void>
  struct has_info_specialization_t : std::false_type
    {
    };

  template<typename enumeration>
  struct has_info_specialization_t<
    enumeration,
    std::void_t<decltype(info<enumeration>::first), decltype(info<enumeration>::last)>> : std::true_type
    {
    };

  template<typename enumeration>
  concept has_info_specialization = has_info_specialization_t<enumeration>::value;

  struct meta_info_bounds_traits
    {
    bool lower_bound;
    bool upper_bound;
    };

  template<typename enumeration, meta_info_bounds_traits bi = meta_info_bounds_traits{}>
  struct meta_info : std::false_type
    {
    static constexpr enumeration first = static_cast<enumeration>(0);
    static constexpr enumeration last = static_cast<enumeration>(default_unbounded_upper_range);
    };

  template<typename enumeration>
  struct meta_info<enumeration, meta_info_bounds_traits{.lower_bound = true, .upper_bound = true}>
    {
    static constexpr enumeration first = enumeration::first;
    static constexpr enumeration last = enumeration::last;
    };

  // default assume range 0 - 10
  template<typename enumeration>
  struct meta_info<enumeration, meta_info_bounds_traits{.lower_bound = true, .upper_bound = false}>
    {
    static constexpr enumeration first = enumeration::first;
    static constexpr enumeration last = static_cast<enumeration>(default_unbounded_upper_range);
    };

  template<typename enumeration>
  struct meta_info<enumeration, meta_info_bounds_traits{.lower_bound = false, .upper_bound = true}>
    {
    static constexpr enumeration first = static_cast<enumeration>(0);
    static constexpr enumeration last = enumeration::last;
    };

  template<typename enumeration>
  struct bounds
    {
    // when user provided info is available is has priority over defaulted specializations of meta_info
    using info_type = std::conditional_t<
      has_info_specialization<enumeration>,
      info<enumeration>,
      meta_info<
        enumeration,
        meta_info_bounds_traits{.lower_bound = lower_bounded_enum<enumeration>, .upper_bound = upper_bounded_enum<enumeration>}>>;

    static constexpr enumeration first = info_type::first;
    static constexpr enumeration last = info_type::last;
    static constexpr auto first_index = simple_enum::to_underlying(info_type::first);
    static constexpr auto last_index = simple_enum::to_underlying(info_type::last);
    };
  }  // namespace detail
  }  // namespace simple_enum::inline v0_4

// this namespace is for reducing time crunching source location and .text data
namespace se
  {
template<auto enumeration>
constexpr auto f() noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
  char const * const func{__FUNCSIG__};
#elif defined(__cpp_lib_source_location)
  char const * const func{std::source_location::current().function_name()};
#else
#error "Can not help you not a clang, gcc or msvc"
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
#if __GNUC__ < 10
#error "Gcc minimal supported version is 10"
#endif
// index of the = character in the given line "constexpr auto se::f() [with auto enumeration =" is 46.
inline constexpr auto initial_offset{46 + 1};
inline constexpr char end_of_enumeration_name = ']';

#elif defined(_MSC_VER)
// index of the < character in the given line "auto __cdecl se::f<" is 18
//                         0x00007ff76b5d33b0 "auto __cdecl se::f<v1>(void) noexcept"
inline constexpr auto initial_offset{18};
inline constexpr char end_of_enumeration_name = '>';
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
#if defined(_MSC_VER)
  size_t pos = func.find('<');
  if(pos == std::string_view::npos)
    throw;
  return pos;
#else
  size_t pos = func.find("enumeration =");
  if(pos == std::string_view::npos)
    throw;
  return pos + 12 + 1;
#endif
  }

auto constexpr verify_offset() -> bool { return find_enumeration_offset() == initial_offset; }

static_assert(verify_offset());
#endif
  }  // namespace se

namespace simple_enum::inline v0_4
  {
namespace detail
  {
  struct meta_name
    {
    char const * data;
    size_t size;
    };

  template<auto enumeration>
  constexpr auto first_pass(meta_name & res) noexcept
    {
    char const * const func{se::f<enumeration>()};
    char const * end_of_name{func + se::initial_offset};
    char const * last_colon{end_of_name};
#ifdef _MSC_VER
    size_t was_undefined{};
#endif
    for(; *end_of_name != se::end_of_enumeration_name; ++end_of_name)
#ifdef _MSC_VER
      if(*end_of_name == ':')
        last_colon = end_of_name;
      else if(*end_of_name == ')')
        {
        last_colon = end_of_name;
        was_undefined = 5;
        }
#else
      if(*end_of_name == ':' || *end_of_name == ')')
        last_colon = end_of_name;
#endif

    res.data = last_colon + 1;
    res.size = size_t(end_of_name - res.data);
#ifdef _MSC_VER
    return size_t(last_colon - func) + 1 - was_undefined;
#else
    return size_t(last_colon - func) + 1;
#endif
    }

  template<auto enumeration>
  constexpr void cont_pass(meta_name & res, std::size_t enum_beg) noexcept
    {
    char const * const func{se::f<enumeration>()};
    char const * end_of_name{func + enum_beg};
    char const * enumeration_name{end_of_name};
    while(*end_of_name != se::end_of_enumeration_name)
      ++end_of_name;  // for other enumerations we only need to find end of string

    res.data = enumeration_name;
    res.size = size_t(end_of_name - res.data);
    }

  template<typename enum_type, std::integral auto first, std::size_t size, typename name_array, std::size_t... indices>
  constexpr void apply_meta_enum(name_array & meta, size_t enum_beg, std::index_sequence<indices...>)
    {
    // unpack and call cont_pass for each index, using fold expression
    (..., (cont_pass<static_cast<enum_type>(first + indices)>(meta[indices + 1], enum_beg)));
    }

  template<typename enum_type, std::integral auto first, std::integral auto last, std::size_t size, typename name_array>
  constexpr void fold_array(name_array & meta)
    {
#ifdef SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS
    static_assert(size == static_cast<std::size_t>(last - first + 1), "size must match the number of enum values");
#endif
    size_t enum_beg{first_pass<static_cast<enum_type>(first)>(meta[0])};
    if constexpr(size > 1)
      apply_meta_enum<enum_type, first + 1, size - 1>(meta, enum_beg, std::make_index_sequence<size - 1>{});
    }

  template<enum_concept enum_type>
  constexpr auto enum_meta_array() noexcept
    {
    using bounds_type = detail::bounds<enum_type>;
    static_constexpr constexpr auto first_index{bounds_type::first_index};
    static_constexpr constexpr auto last_index{bounds_type::last_index};
    std::array<detail::meta_name, last_index - first_index + 1> meta;
    detail::fold_array<enum_type, first_index, last_index, last_index - first_index + 1>(meta);
    return meta;
    }
  }  // namespace detail

template<typename enumeration>
concept bounded_enum = detail::has_info_specialization<enumeration> || detail::internaly_bounded_enum<enumeration>;

struct enum_name_t
  {
  template<enum_concept enum_type>
  static_call_operator constexpr auto operator()(enum_type value) static_call_operator_const noexcept
    -> std::string_view
    {
    using bounds_type = detail::bounds<enum_type>;
    static_constexpr constexpr auto first_index{bounds_type::first_index};
    static_constexpr constexpr auto last_index{bounds_type::last_index};
    static_constexpr constexpr auto meta{detail::enum_meta_array<enum_type>()};
    auto const requested_index{simple_enum::to_underlying(value)};
    if(requested_index >= first_index && requested_index <= last_index)
      {
      detail::meta_name const & res{meta[size_t(requested_index - first_index)]};
      return std::string_view{res.data, res.size};
      }
    else
      return {""};  // return empty but null terminated
    }
  };

inline constexpr enum_name_t enum_name;

  }  // namespace simple_enum::inline v0_4

#pragma pop_macro("static_call_operator_const")
#pragma pop_macro("static_call_operator")
#pragma pop_macro("static_constexpr")
