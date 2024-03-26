// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum

#pragma once
#include <simple_enum/simple_enum.hpp>
#include <simple_enum/enum_cast.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wundefined-func-template"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

#include <glaze/glaze.hpp>

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#include <concepts>

namespace simple_enum::inline v0_7
  {
template<bounded_enum enumeration_type>
static constexpr auto enum_names_array = []()
{
  constexpr auto size = detail::enum_base_info_t<enumeration_type>::size();
  std::array<std::string_view, size> names;
  for(uint16_t ix{}; size != ix; ++ix)
    names[ix] = detail::enum_meta_info_t<enumeration_type>::meta_data[ix].as_view();
  return names;
}();

template<bounded_enum enumeration_type>
static constexpr auto enum_values_array = []()
{
  constexpr auto size = detail::enum_base_info_t<enumeration_type>::size();
  std::array<enumeration_type, size> values;
  for(size_t ix{}; size != ix; ++ix)
    values[ix] = static_cast<enumeration_type>(ix + detail::enum_base_info_t<enumeration_type>::first_index());
  return values;
}();

namespace detail
  {
  // Interleave Helper: Interleaves two sequences at compile time into a tuple
  template<typename Array1, typename Array2, std::size_t... I>
  constexpr auto interleave_impl(Array1 const & a1, Array2 const & a2, std::index_sequence<I...>)
    {
    return std::tuple_cat(std::make_tuple(std::get<I>(a1), std::get<I>(a2))...);
    }
  }  // namespace detail

// Interleave: Creates a tuple of interleaved elements from two arrays
template<typename Array1, typename Array2>
constexpr auto interleave(Array1 const & a1, Array2 const & a2)
  {
  return detail::interleave_impl(a1, a2, std::make_index_sequence<std::tuple_size_v<Array1>>{});
  }
  }  // namespace simple_enum::inline v0_7

namespace simple_enum::inline v0_7::concepts
  {
template<typename T>
concept read_json_supported = glz::detail::
  read_json_invocable<glz::opts{}, std::add_lvalue_reference<T>, glz::context &, char const *&, char const *&>;

template<typename T>
concept write_json_supported = glz::detail::write_json_invocable<
  glz::opts{},
  std::add_const_t<std::add_lvalue_reference<T>>,
  glz::context &,
  std::string &,
  size_t &>;
  }  // namespace simple_enum::inline v0_7::concepts

namespace glz
  {
template<simple_enum::enum_concept enumeration_type>
struct meta<enumeration_type>
  {
  static constexpr bool custom_write = true;
  static constexpr bool custom_read = true;

  static constexpr auto color_values{simple_enum::enum_names_array<enumeration_type>};
  static constexpr auto color_names{simple_enum::enum_values_array<enumeration_type>};
  static constexpr auto interleaved = simple_enum::interleave(color_names, color_values);
  static constexpr std::string_view name = simple_enum::enumeration_name_v<enumeration_type>;
  static constexpr auto value = std::apply(
    [](auto &&... args) noexcept { return glz::enumerate_no_reflect(std::forward<decltype(args)>(args)...); },
    interleaved
  );
  };
  }  // namespace glz

namespace glz::detail
  {

template<simple_enum::enum_concept enumeration_type>
struct from_json<enumeration_type>
  {
  static_assert(simple_enum::bounded_enum<enumeration_type>);

  template<auto Opts>
  static void op(enumeration_type & arg, is_context auto && ctx, auto &&... args)
    {
    std::string_view value;
    read<json>::op<Opts>(value, ctx, args...);

    if(bool(ctx.error)) [[unlikely]]
      return;

    cxx23::expected<enumeration_type, simple_enum::enum_cast_error> res{simple_enum::enum_cast<enumeration_type>(value)
    };
    if(!res.has_value()) [[unlikely]]
      {
      ctx.error = error_code::syntax_error;
      return;
      }
    arg = res.value();
    }
  };

template<simple_enum::enum_concept enumeration_type>
struct to_json<enumeration_type>
  {
  static_assert(simple_enum::bounded_enum<enumeration_type>);

  template<auto Opts>
  static void op(enumeration_type const & arg, auto &&... args) noexcept
    {
    std::string_view value{simple_enum::enum_name(arg)};
    write<json>::op<Opts>(value, args...);
    }
  };
  }  // namespace glz::detail
