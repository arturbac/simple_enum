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

consteval auto adl_enum_bounds(test_enum_e)
  {
  using enum test_enum_e;
  return simple_enum::adl_info{foo, baz};
  }

static_assert(simple_enum::concepts::read_json_supported<test_enum_e>);
static_assert(simple_enum::concepts::write_json_supported<test_enum_e>);

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
    expect(err.ec == glz::error_code::none);

    auto str = glz::write_json(data);
    expect(eq("{\"enum_field\":\"baz\"}"sv, str));
  };
  "read_file_json test"_test = []
  {
    test_data_t data{test_enum_e::foo};
    auto err{glz::read_file_json(data, std::string{"rboxes_file_name"}, std::string{})};
    expect(err.ec == glz::error_code::none);
    expect(data.enum_field == test_enum_e::baz);
  };
  }
