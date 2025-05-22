// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum

#pragma once

#ifdef SIMPLE_ENUM_CXX_MODULE
import simple_enum;
#else
#include <simple_enum/simple_enum.hpp>
#include <simple_enum/enum_cast.hpp>
#include <simple_enum/indexed_access.hpp>
#endif

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

namespace simple_enum::inline v0_9
  {

template<bounded_enum enumeration_type>
static constexpr auto enum_names_array = []()
{
  constexpr auto size = enum_size<enumeration_type>();
  std::array<std::string_view, size> names;
  for(uint16_t ix{}; size != ix; ++ix)
    names[ix] = enum_name_at_index<enumeration_type>(ix);
  return names;
}();

template<bounded_enum enumeration_type>
static constexpr auto enum_values_array = []()
{
  constexpr auto size = enum_size<enumeration_type>();
  std::array<enumeration_type, size> values;
  for(size_t ix{}; size != ix; ++ix)
    values[ix] = enum_value_at_index<enumeration_type>(ix);
  return values;
}();

#ifdef glaze_v4_2_3_tuple
namespace glz_tuple_ns = glz;
#else
namespace glz_tuple_ns = glz::tuplet;
#endif
namespace detail
  {

  template<typename... Args>
  constexpr auto convert_to_glz_enum(std::tuple<Args...> const & stdTuple)
    {
    return glz::detail::Enum{
      std::apply([](auto &&... args) { return glz_tuple_ns::tuple<std::decay_t<Args>...>{args...}; }, stdTuple)
    };
    }

  template<typename enumeration_type, std::size_t... ix>
  constexpr auto glaze_tuple_pairs(std::index_sequence<ix...>)
    {
    return std::tuple_cat(
      std::make_tuple(enum_name_at_index<enumeration_type>(ix), enum_value_at_index<enumeration_type>(ix))...
    );
    }

  template<typename enumeration_type>
  constexpr auto make_glaze_tuple()
    {
    constexpr auto size = enum_size<enumeration_type>();
    return convert_to_glz_enum(glaze_tuple_pairs<enumeration_type>(std::make_index_sequence<size>{}));
    }

  }  // namespace detail
  }  // namespace simple_enum::inline v0_9

template<simple_enum::bounded_enum enumeration_type>
struct glz::meta<enumeration_type>
  {
  static constexpr bool custom_write = true;
  static constexpr bool custom_read = true;

  static constexpr std::string_view name = simple_enum::enumeration_name_v<enumeration_type>;
  static constexpr auto value = simple_enum::detail::make_glaze_tuple<enumeration_type>();
  };

#ifdef glaze_v5_0_0
namespace glz_to_from_ns = glz;
#else
namespace glz_to_from_ns = glz::detail;
#endif

template<simple_enum::bounded_enum enumeration_type>
#ifdef glaze_v3_5_0_to_from
struct glz_to_from_ns::from<glz::JSON, enumeration_type>
#else
struct from_json<enumeration_type>
#endif
  {
  template<auto Opts, typename... Args>
    requires simple_enum::bounded_enum<enumeration_type>
  static void op(enumeration_type & arg, is_context auto && ctx, Args &&... args)
    {
    std::string_view value;
#ifdef glaze_v3_5_0_to_from
#ifdef glaze_v5_0_0_generic_supported
    parse<glz::JSON>::op<Opts>(value, ctx, args...);
#else
    read<glz::JSON>::op<Opts>(value, ctx, args...);
#endif
#else
    read<glz::json>::op<Opts>(value, ctx, args...);
#endif
    if(bool(ctx.error)) [[unlikely]]
      return;

    simple_enum::expected<enumeration_type, simple_enum::enum_cast_error> res{
      simple_enum::enum_cast<enumeration_type>(value)
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
#ifdef glaze_v3_5_0_to_from
struct glz_to_from_ns::to<glz::JSON, enumeration_type>
#else
struct to_json<enumeration_type>
#endif
  {
  template<auto Opts>
    requires simple_enum::bounded_enum<enumeration_type>
  static void op(enumeration_type const & arg, auto &&... args) noexcept
    {
    std::string_view value{simple_enum::enum_name(arg)};
#ifdef glaze_v3_5_0_to_from
#ifdef glaze_v5_0_0_generic_supported
    serialize<glz::JSON>::op<Opts>(value, args...);
#else
    write<glz::JSON>::op<Opts>(value, args...);
#endif
#else
    write<glz::json>::op<Opts>(value, args...);
#endif
    }
  };

