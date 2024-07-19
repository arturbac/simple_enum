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
namespace detail
  {
  template<bounded_enum enumeration_type>
  constexpr auto enum_value_at_index(size_t ix) -> enumeration_type
    {
    using underlying_type = std::underlying_type_t<enumeration_type>;
    return static_cast<enumeration_type>(
      detail::enum_base_info_t<enumeration_type>::first_index() + underlying_type(ix)
    );
    }

  template<bounded_enum enumeration_type>
  constexpr auto enum_name_at_index(size_t ix) -> std::string_view
    {
    return detail::enum_meta_info_t<enumeration_type>::meta_data[ix].as_view();
    }
  }  // namespace detail

template<bounded_enum enumeration_type>
static constexpr auto enum_names_array = []()
{
  constexpr auto size = detail::enum_base_info_t<enumeration_type>::size();
  std::array<std::string_view, size> names;
  for(uint16_t ix{}; size != ix; ++ix)
    names[ix] = detail::enum_name_at_index<enumeration_type>(ix);
  return names;
}();

template<bounded_enum enumeration_type>
static constexpr auto enum_values_array = []()
{
  constexpr auto size = detail::enum_base_info_t<enumeration_type>::size();
  std::array<enumeration_type, size> values;
  for(size_t ix{}; size != ix; ++ix)
    values[ix] = detail::enum_value_at_index<enumeration_type>(ix);
  return values;
}();

namespace detail
  {
  template<typename Array1, typename Array2, std::size_t... I>
  constexpr auto interleave_impl(Array1 const & a1, Array2 const & a2, std::index_sequence<I...>)
    {
    return std::tuple_cat(std::make_tuple(std::get<I>(a1), std::get<I>(a2))...);
    }

  template<typename enumeration_type>
  constexpr auto make_glz_enum_tuple(std::string_view string_value, enumeration_type enumeration)
    -> glz::tuplet::tuple<std::string_view, enumeration_type>
    {
    return glz::tuplet::tuple<std::string_view, enumeration_type>{string_value, enumeration};
    }

#define GLZ_3_1_x

#if defined(GLZ_2_x)
  template<typename Array1, typename Array2, std::size_t... I>
  constexpr auto make_glaze_tuple_impl(Array1 const & a1, Array2 const & a2, std::index_sequence<I...>)
    {
    return std::make_tuple(make_glz_enum_tuple(a1[I], a2[I])...);
    }
#elif defined(GLZ_3_1_x)
  template<typename Array1, typename Array2, std::size_t... I>
  constexpr auto make_glaze_tuple_impl(Array1 const & a1, Array2 const & a2, std::index_sequence<I...>)
    {
    return std::tuple_cat(std::make_tuple(a1[I], a2[I])...);
    }
#endif
  template<typename Array1, typename Array2>
  constexpr auto make_glaze_tuple(Array1 const & a1, Array2 const & a2)
    {
    constexpr auto size1 = std::tuple_size<std::decay_t<Array1>>::value;
    constexpr auto size2 = std::tuple_size<std::decay_t<Array2>>::value;
    static_assert(size1 == size2);
    return make_glaze_tuple_impl(a1, a2, std::make_index_sequence<size1>{});
    }

  template<typename... Args>
  constexpr auto convert_to_glz_enum(std::tuple<Args...> const & stdTuple)
    {
    return glz::detail::Enum{
      std::apply([](auto &&... args) { return glz::tuplet::tuple<std::decay_t<Args>...>{args...}; }, stdTuple)
    };
    }  // namespace detail

  template<typename Array1, typename Array2>
  constexpr auto interleave(Array1 const & a1, Array2 const & a2)
    {
    return detail::interleave_impl(a1, a2, std::make_index_sequence<std::tuple_size_v<Array1>>{});
    }

  }  // namespace detail
  }  // namespace simple_enum::inline v0_7

namespace simple_enum::inline v0_7::concepts
  {
using glz::read_json_supported;
using glz::write_json_supported;

  }  // namespace simple_enum::inline v0_7::concepts

namespace glz
  {
template<simple_enum::bounded_enum enumeration_type>
struct meta<enumeration_type>
  {
  static constexpr bool custom_write = true;
  static constexpr bool custom_read = true;

  static constexpr std::string_view name = simple_enum::enumeration_name_v<enumeration_type>;
  static constexpr auto value = simple_enum::detail::convert_to_glz_enum(
    simple_enum::detail::
      make_glaze_tuple(simple_enum::enum_names_array<enumeration_type>, simple_enum::enum_values_array<enumeration_type>)
  );
  };

  }  // namespace glz

namespace glz::detail
  {

template<simple_enum::bounded_enum enumeration_type>
struct from_json<enumeration_type>
  {
  template<auto Opts>
    requires simple_enum::bounded_enum<enumeration_type>
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

template<simple_enum::bounded_enum enumeration_type>
struct to_json<enumeration_type>
  {
  template<auto Opts>
    requires simple_enum::bounded_enum<enumeration_type>
  static void op(enumeration_type const & arg, auto &&... args) noexcept
    {
    std::string_view value{simple_enum::enum_name(arg)};
    write<json>::op<Opts>(value, args...);
    }
  };
  }  // namespace glz::detail

