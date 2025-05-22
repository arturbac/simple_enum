# `enum_bitfield_t`

## Overview

The `enum_bitfield_t` is a size adaptive bitfiled strage with using enumeration as an indexing type for bitfields
storage type depends on value of enum_size_v<> for enum selecting minimal suitable unisgned intergral

Include the `simple_enum/enum_bitfield.h` in your project to use `enum_bitfield_t`

## `enum_index` Interface

```cpp

template<enum_concept enum_type_t>
struct enum_bitfield_t;

```

## Example Usage

```cpp
enum struct color_t { red, green, blue, yellow, first = red, last = yellow };
{
  enum_bitfield_t<color_t> colors;
  colors[color_t::green] = true;
  expect(colors[color_t::green]);
  expect(not colors[color_t::red]);
  expect(not colors[color_t::yellow]);
}
{
  // construction with varidic number of arguments
  enum_bitfield_t<color_t> colors{color_t::red, color_t::blue};
  expect(colors[color_t::blue]);
  expect(not colors[color_t::yellow]);
  expect(not colors[color_t::green]);
  expect(colors[color_t::red]);
  colors[color_t::blue] = false;
  expect(not colors[color_t::blue]);
}
```

