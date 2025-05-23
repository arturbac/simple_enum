// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#ifndef SIMPLE_ENUM_CXX_MODULE
#include <simple_enum/enum_index.hpp>
#endif
#include "simple_enum_tests.hpp"

using simple_enum::enum_index;
using simple_enum::enum_index_error;
using simple_enum::enum_size_v;

enum struct E1 : uint8_t
  {
  _1,
  _2,
  _3,
  _4,
  _5,
  first = _1,
  last = _5
  };
enum struct E2 : uint8_t
  {
  _1 = 10,
  _2,
  _3,
  _4,
  _5,
  first = _1,
  last = _5
  };
static_assert(enum_size_v<E> == 16);
static_assert(enum_size_v<strong_typed> == 3);
static_assert(enum_size_v<E1> == 5);
static_assert(enum_size_v<E2> == 5);

int main()
  {
  "enum_index valid"_test = []
  {
    using std::to_underlying;

    expect(eq(enum_index(lorem_ipsum_short::eu).value(), to_underlying(lorem_ipsum_short::eu)));
    expect(eq(enum_index(lorem_ipsum_short::adipiscing).value(), to_underlying(lorem_ipsum_short::adipiscing)));
    expect(eq(enum_index(lorem_ipsum_short::exercitation).value(), to_underlying(lorem_ipsum_short::exercitation)));

    expect(eq(enum_index(weak::weak_typed_e::v2).value(), 1u));
    expect(eq(enum_index(test::subnamespace::example_3_e::v2).value(), 1u));
#ifndef SIMPLE_ENUM_CXX_MODULE
      {
      using mf = simple_enum::detail::enum_meta_info_t<weak::weak_typed_e>;
      static_assert(static_cast<weak::weak_typed_e>(mf::first_index()) == weak::v1);
      static_assert(static_cast<weak::weak_typed_e>(mf::last_index()) == weak::v3);
      }
#endif
  };

  "enum_index out_of_range"_test = []
  {
    using std::to_underlying;
    auto out_of_range_value = static_cast<lorem_ipsum_short>(to_underlying(lorem_ipsum_short::last) + 1);
    expect(enum_index(out_of_range_value).error() == enum_index_error::out_of_range);
  };
  }

