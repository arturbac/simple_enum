// SPDX-FileCopyrightText: 2024 Artur Bać
// SPDX-License-Identifier: BSL-1.0
// SPDX-PackageHomePage: https://github.com/arturbac/simple_enum
#ifndef SIMPLE_ENUM_CXX_MODULE
#include <simple_enum/enum_bitfield.h>
#endif
#include "simple_enum_tests.hpp"

using simple_enum::enum_bitfield_t;
using simple_enum::enum_size_v;

// clang-format off
// Enum that should use uint8_t
enum struct color_t { red, green, blue, yellow, first = red, last = yellow };

// Enum that should also use uint16_t 16 - 4 + 1
enum struct permission_t { read = 4, write = 8, execute = 16, first = read, last = execute };

// Enum that should use uint16_t (9 values)
enum struct medium_enum_t { v0, v1, v2, v3, v4, v5, v6, v7, v8, first = v0, last = v8 };

// Enum that should use uint64_t (40 values)
enum struct large_enum_t {
    v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
    v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
    v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
    v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
    first = v0, last = v39
};
// clang-format on

int main()
  {
  "enum_bitfield"_test = []
  {
    "bitfield_logic"_test = []
    {
      enum_bitfield_t<color_t> colors;
      colors[color_t::green] = true;
      expect(colors[color_t::green]);
      expect(not colors[color_t::red]);
      expect(not colors[color_t::yellow]);

      colors[color_t::yellow] = true;

      expect(colors[color_t::green]);
      expect(colors[color_t::yellow]);
      expect(not colors[color_t::red]);

      colors[color_t::green] = false;
      expect(not colors[color_t::green]);

      enum_bitfield_t<color_t> const copy{colors};
      // copy[color_t::green] = true; parent should be const
      auto proxy{copy[color_t::green]};
      static_assert(std::is_reference_v<decltype(proxy.parent_)>);
      static_assert(std::is_const_v<std::remove_reference_t<decltype(proxy.parent_)>>);

      expect(not copy[color_t::green]);
      expect(copy[color_t::yellow]);
      expect(not copy[color_t::red]);

      colors[color_t::yellow] = false;
      colors[color_t::red] = true;
      expect(not colors[color_t::green]);
      expect(not colors[color_t::yellow]);
      expect(colors[color_t::red]);
    };

    "storage_size"_test = []
    {
      // Test case 1: 4 values -> should use uint8_t
      expect(enum_size_v<color_t> == 4);
      using color_bitfield_t = enum_bitfield_t<color_t>;
      static_assert(std::is_same_v<color_bitfield_t::storage_t, uint8_t>);
      expect(sizeof(color_bitfield_t) == sizeof(uint8_t));

      expect(enum_size_v<permission_t> == 13);
      using permission_bitfield_t = enum_bitfield_t<permission_t>;
      static_assert(std::is_same_v<permission_bitfield_t::storage_t, uint16_t>);
      expect(sizeof(permission_bitfield_t) == sizeof(uint16_t));

      // Test case 2: 9 values -> should use uint16_t
      expect(enum_size_v<medium_enum_t> == 9);
      using medium_bitfield_t = enum_bitfield_t<medium_enum_t>;
      static_assert(std::is_same_v<medium_bitfield_t::storage_t, uint16_t>);
      expect(sizeof(medium_bitfield_t) == sizeof(uint16_t));

      // Test case 3: 40 values -> should use uint64_t
      expect(enum_size_v<large_enum_t> == 40);
      using large_bitfield_t = enum_bitfield_t<large_enum_t>;
      static_assert(std::is_same_v<large_bitfield_t::storage_t, uint64_t>);
      expect(sizeof(large_bitfield_t) == sizeof(uint64_t));
    };

    "large_bitfield_logic"_test = []
    {
      enum_bitfield_t<large_enum_t> large_bf;

      // Test a bit at a high index (> 32)
      large_bf[large_enum_t::v35] = true;
      large_bf[large_enum_t::v39] = true;

      expect(large_bf[large_enum_t::v35]);
      expect(large_bf[large_enum_t::v39]);
      expect(not large_bf[large_enum_t::v0]);
      expect(not large_bf[large_enum_t::v38]);

      // Read via proxy
      bool const has_v35 = large_bf[large_enum_t::v35];
      expect(has_v35);

      // Clear bit and re-verify
      large_bf[large_enum_t::v35] = false;
      expect(not large_bf[large_enum_t::v35]);
    };

    "bitfield_multi_construct"_test = []
    {
      {
      enum_bitfield_t<color_t> colors{color_t::red};
      expect(not colors[color_t::green]);
      expect(not colors[color_t::yellow]);
      expect(colors[color_t::red]);
      }
      {
      enum_bitfield_t<color_t> colors{color_t::red, color_t::green};
      expect(colors[color_t::green]);
      expect(not colors[color_t::yellow]);
      expect(colors[color_t::red]);
      }
      {
      using enum large_enum_t;
      enum_bitfield_t<large_enum_t> large_bf{v35, v31, v12, v17, v1};
      expect(not large_bf[v34]);
      expect(large_bf[v35]);
      expect(not large_bf[v36]);

      expect(not large_bf[v30]);
      expect(large_bf[v31]);
      expect(not large_bf[v32]);

      expect(not large_bf[v16]);
      expect(large_bf[v17]);
      expect(not large_bf[v18]);

      expect(large_bf[v1]);
      expect(not large_bf[v2]);
      }
    };
  };
  }

