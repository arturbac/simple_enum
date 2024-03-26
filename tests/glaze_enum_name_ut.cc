// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#include <simple_enum/glaze_json_enum_name.hpp>
#include <simple_enum/ranges_views.hpp>
#include "simple_enum_tests.hpp"
#include <tuple>
#include <ranges>
#include <utility>
#include <algorithm>

namespace views = std::views;
namespace ranges = std::ranges;
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
enum class Color
  {
  Red,
  Green,
  Blue
  };

consteval auto adl_enum_bounds(Color)
  {
  using enum Color;
  return simple_enum::adl_info{Red, Blue};
  }

// Template to generate a compile-time sequence of pairs from two arrays
template<auto & Values, auto & Names, std::size_t... Is>
constexpr auto make_enumeration_seq(std::index_sequence<Is...>)
  {
  return std::tuple(std::make_pair(Names[Is], Values[Is])...);
  }

// Utility to create the enumeration sequence
template<auto & Values, auto & Names>
constexpr auto make_enumeration()
  {
  return make_enumeration_seq<Values, Names>(std::make_index_sequence<Values.size()>{});
  }

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

  "enumerate test"_test = []
  {
    std::string schema = glz::write_json_schema<test_data_t>();
    // expect(false) << schema;
    expect(eq(
      schema,
      R"({"type":["object"],"properties":{"enum_field":{"$ref":"#/$defs/test_enum_e"}},"additionalProperties":false,"$defs":{"test_enum_e":{"type":["string"],"oneOf":[{"description":"foo","const":"foo"},{"description":"bar","const":"bar"},{"description":"baz","const":"baz"}]}}})"sv
    ));
    //  auto color_values{simple_enum::enum_names_array<Color>};
    //  auto color_names{simple_enum::enum_values_array<Color>};
    //  auto interleaved = simple_enum::interleave(color_names, color_values);
    //  std::string_view name = simple_enum::enumeration_name_v<Color>;
    //  auto value = std::apply(
    //   [](auto &&... args) noexcept { return glz::enumerate(std::forward<decltype(args)>(args)...); }, interleaved
    // );
    // constexpr auto color_values{simple_enum::enum_enumerations<Color>()};
    // constexpr auto color_names{simple_enum::enum_names<Color>()};
  };
  }
