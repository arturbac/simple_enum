# `enum_bitfield_t`

## Overview

The `enum_bitfield_t` is a size adaptive bitfiled strage with using enumeration as an indexing type for bitfields
storage type depends on value of enum_size_v<> for enum selecting minimal suitable unisgned intergral

Include the `simple_enum/enum_bitfield.h` in your project to use `enum_bitfield_t`

## `enum_bitfield_t` Interface


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
  
  expect(enum_bitfield_t{red} != enum_bitfield_t{green});
  static_assert(enum_bitfield_t{yellow, red} == enum_bitfield_t{yellow, red});
  
}
```
enum_bitfield_t support class template argument deducation when using arguments
```cpp
{
  // construction with varidic number of arguments
  enum_bitfield_t colors{color_t::red, color_t::blue};
  expect(colors[color_t::blue]);
  expect(not colors[color_t::yellow]);
  expect(not colors[color_t::green]);
  expect(colors[color_t::red]);
  colors[color_t::blue] = false;
  expect(not colors[color_t::blue]);
}
```

```cpp
  // operators |, &, |= &=
  static_assert((enum_bitfield_t{v16, v21} | enum_bitfield_t{v31, v5}) 
                                        == enum_bitfield_t{v21, v16, v5, v31});
  static_assert((enum_bitfield_t{v16, v21, v34, v1} & enum_bitfield_t{v16, v21, v5})
                                        == enum_bitfield_t{v21, v16});
  
  enum_bitfield_t a{v36};
  a &= enum_bitfield_t{v21};
  expect(a == enum_bitfield_t<large_enum_t>{});
  
  enum_bitfield_t b{v16, v21};
  b |= enum_bitfield_t{v31, v5};
  expect(b == enum_bitfield_t{v21, v16, v5, v31});
```

