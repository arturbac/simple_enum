#pragma once

#include <simple_enum/simple_enum.hpp>
#include <fmt/format.h>
#include <type_traits>

namespace fmt
  {

template<typename T>
  requires simple_enum::enum_concept<T>
struct formatter<T>
  {
  constexpr auto parse(format_parse_context & ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

  template<typename format_context>
  auto format(T const & e, format_context & ctx) const -> decltype(ctx.out())
    {
    return fmt::format_to(ctx.out(), "{}", simple_enum::enum_name(e));
    }
  };

  }  // namespace fmt

