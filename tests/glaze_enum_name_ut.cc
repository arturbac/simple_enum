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
    // expect(false) << schema;
    expect(eq(
      schema,
      R"({"type":["object"],"properties":{"enum_field":{"$ref":"#/$defs/test_enum_e"}},"additionalProperties":false,"$defs":{"test_enum_e":{"type":["string"],"oneOf":[{"const":"foo"},{"const":"bar"},{"const":"baz"}]}}})"sv
    ));
  };
  "json rpc call test"_test = []
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
    // expect(false) << request_str;
    expect(eq(R"({"jsonrpc":"2.0","method":"foo","params":{"enum_field":"bar"},"id":"42"})"sv, request_str));
    std::string response = server.call(request_str);
    // expect(false) << response;
    expect(eq(R"({"jsonrpc":"2.0","result":{"enum_field":"baz"},"id":"42"})"sv, response));
  };
  }

