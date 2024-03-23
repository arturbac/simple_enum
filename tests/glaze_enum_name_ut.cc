// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#include <simple_enum/glaze_json_enum_name.hpp>
#include "simple_enum_tests.hpp"

enum struct test_enum_e
  {
  foo,
  bar,
  baz
  };

struct test_data_t
  {
  test_enum_e enum_field;
  };

inline constexpr glz::opts pretty{.prettify = true};

int main()
  {
  "write_file_json test"_test = []
  {
    constexpr test_data_t data{.enum_field = test_enum_e::baz};
    glz::write_error err{glz::write_file_json<pretty>(data, std::string{"rboxes_file_name"}, std::string{})};
  };
  }

/*
if constexpr (write_json_invocable<Opts, T, Ctx, B, IX>) {
               to_json<std::remove_cvref_t<T>>::template op<Opts>(std::forward<T>(value), std::forward<Ctx>(ctx),
                                                                  std::forward<B>(b), std::forward<IX>(ix));
error: static assertion failed due to requirement 'false_v<const test_enum_e &>': Glaze metadata is probably needed for
your type te T type is from get_member(value, member) and it is  test_enum_e const &

// OK no test
write<json>::op<Opts>(get_member(value, member), ctx, b, ix);
glz::detail::write<10>::op<opts{}, const test_enum_e &, glz::context &, std::string &, unsigned long &>

// Goes thru op_base<write_unknown_on<Options>()>(std::forward<V>(value), ctx, b, ix);
glz::detail::to_json<test_data_t>::op_base<opts{}, const test_data_t &, glz::context &, std::string &, unsigned long &>

//OK test if constexpr (write_json_invocable<Opts, T, Ctx, B, IX>) {
glz::detail::to_json<test_data_t>::op<opts{}, const test_data_t &, glz::context &, std::string &, unsigned long &>*/
