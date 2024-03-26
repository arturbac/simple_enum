// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#include <simple_enum/glaze_json_enum_name.hpp>
#include <glaze/ext/jsonrpc.hpp>
#include <simple_enum/ranges_views.hpp>
#include "simple_enum_tests.hpp"
#include <tuple>
#include <ranges>
#include <utility>
#include <algorithm>

namespace glz::rpc
  {
// consteval auto adl_enum_bounds(error_e)
// {
//   using enum error_e;
//   return simple_enum::adl_info{server_error_lower, parse_error};
// }
  }

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

  "write_json_schema test"_test = []
  {
    std::string schema = glz::write_json_schema<test_data_t>();
    expect(false) << schema;
    expect(eq(
      schema,
      R"({"type":["object"],"properties":{"enum_field":{"$ref":"#/$defs/test_enum_e"}},"additionalProperties":false,"$defs":{"test_enum_e":{"type":["string"],"oneOf":[{"description":"foo","const":"foo"},{"description":"bar","const":"bar"},{"description":"baz","const":"baz"}]}}})"sv
    ));
    auto color_values{simple_enum::enum_values_array<Color>};
    auto color_names{simple_enum::enum_names_array<Color>};
    auto interleaved = simple_enum::interleave(color_names, color_values);
    std::string_view name = simple_enum::enumeration_name_v<Color>;
    auto value = std::apply(
      [](auto &&... args) noexcept { return glz::enumerate_no_reflect(std::forward<decltype(args)>(args)...); },
      interleaved
    );
    auto value2 = simple_enum::convert_to_glz_tuple(simple_enum::make_glaze_tuple(color_names, color_values));

    glz::detail::Enum<glz::tuplet::tuple<std::tuple<
      glz::tuplet::tuple<std::basic_string_view<char>, Color>,
      glz::tuplet::tuple<std::basic_string_view<char>, Color>,
      glz::tuplet::tuple<std::basic_string_view<char>, Color>>>>
      value2a;
    glz::detail::Enum<glz::tuplet::tuple<
      glz::tuplet::tuple<std::basic_string_view<char>, Color>,
      glz::tuplet::tuple<std::basic_string_view<char>, Color>,
      glz::tuplet::tuple<std::basic_string_view<char>, Color>>>
      value3;

    auto valuea = glz::enumerate(
      "Red",
      Color::Red,  //
      "Green",
      Color::Green,  //
      "Blue",
      Color::Blue  //
    );
    // constexpr auto color_values{simple_enum::enum_enumerations<Color>()};
    // constexpr auto color_names{simple_enum::enum_names<Color>()};

    std::tuple<
      std::pair<Color, std::basic_string_view<char, std::char_traits<char>>>,
      std::pair<Color, std::basic_string_view<char, std::char_traits<char>>>,
      std::pair<Color, std::basic_string_view<char, std::char_traits<char>>>>
      value4;
  };
  "write_json_schema test"_test = []
  {
    glz::rpc::server<glz::rpc::method<"foo", test_data_t, test_data_t>> server;
    glz::rpc::client<glz::rpc::method<"foo", test_data_t, test_data_t>> client;

    server.on<"foo">([](test_data_t const & params) { return test_data_t{.enum_field = test_enum_e::baz}; });
    std::string uuid{"42"};
    auto [request_str, inserted] = client.request<"foo">(
      uuid,
      test_data_t{.enum_field = test_enum_e::bar},
      [](glz::expected<test_data_t, glz::rpc::error> value, glz::rpc::id_t id) -> void
      {
        // Access to value and/or id
      }
    );
    expect(false) << request_str;
    std::string response = server.call(request_str);
    expect(false) << response;
  };
  }
