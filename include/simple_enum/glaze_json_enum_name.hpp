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
enum struct test_enum_e
  {
  foo,
  bar,
  baz
  };

namespace glz
  {
template<simple_enum::bounded_enum enumeration_type>
struct meta<enumeration_type>
  {
  static constexpr bool custom_write = true;
  static constexpr bool custom_read = true;
  };
  }  // namespace glz

namespace glz::detail
  {

template<simple_enum::bounded_enum enumeration_type>
struct from_json<enumeration_type>
  {
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

static_assert(simple_enum::concepts::read_json_supported<test_enum_e>);

template<simple_enum::bounded_enum enumeration_type>
struct to_json<enumeration_type>
  {
  template<auto Opts>
  static void op(enumeration_type const & arg, auto &&... args) noexcept
    {
    std::string_view value{simple_enum::enum_name(arg)};
    write<json>::op<Opts>(value, args...);
    }
  };

static_assert(simple_enum::concepts::write_json_supported<test_enum_e>);
// template <auto Opts, class T, class Ctx, class B, class IX>
// static_assert(write_json_invocable<glz::opts{},test_enum, glz::context, std::string, int>);

  }  // namespace glz::detail
