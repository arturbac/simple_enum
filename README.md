![MIT](https://img.shields.io/badge/license-MIT-blue.svg) [![CI](https://github.com/arturbac/simple_enum/actions/workflows/ci.yml/badge.svg)](https://github.com/arturbac/simple_enum/actions/workflows/ci.yml)
![C++](https://img.shields.io/badge/C%2B%2B-20%20%7C%2023-blue.svg) ![Version](https://badge.fury.io/gh/arturbac%2Fsimple_enum.svg)


# simple_enum

# features

This project offers efficien enum_name
```cpp

template<typename type>
concept enum_concept = std::is_enum_v<type>;

template<enum_concept enum_type>
constexpr auto enum_name(enum_type value) noexcept -> std::string_view;

```

Motivation to start this project was lack of efficient meta enum_name available.
This is a prof of concept that enum_name can be efficient when enum is bounded so short instantiation can be achieved and cost can be proportional to number of enumerations in enum.

simple_enum is under active development version checked only on clang 17 and gcc 13, msvc support is coming soon.

There is no runtime cost at all with getting enum_name for runtime variable, the same as in magic_enum.
But computation at compile time is limitted to number of elements in a range thanks to bounded_enum concept if declared for enum type. Unbounded enums are supported too with assumption of simple_enum::default_unbounded_uuper_range which can be ovveriden by user.
There are optimizations related to enum name subtraction, only one loop is performed, and after first enumeration parsed rest of enumerations are parsed with reduced string literal range.
# example
```cpp
enum struct enum_bounded
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };
  
  // can be evaluated at compile time
  static_assert(simple_enum::enum_name(enum_bounded::v2) == "v2");
  // or at runtime
  auto x0{enum_bounded::v2};
  // enum_bounded has definitions for first and last so compile time is limited to processing meta info for declared
  // range only
  ut::expect(simple_enum::enum_name(x0) == "v2");
  // enum_upper_bounded has definitions for last so compile time is limited to processing meta info for range
  // [0..last] range only for upper bounded enum may be sparse enum used with not present first elements including 0
  auto x1{enum_upper_bounded::v2};
  ut::expect(simple_enum::enum_name(x1) == "v2");
  
// lets see example for std::memory_order externally declaring boundary

enum class std::memory_order : int
  {
  relaxed,
  //[..]
  seq_cst
  };
  
template<>
struct simple_enum::info<std::memory_order>
  {
  static constexpr auto first = std::memory_order::relaxed;
  static constexpr auto last = std::memory_order::seq_cst;
  };
  
  auto x1{std::memory_order::release};
  ut::expect(simple_enum::enum_name(x1) == "release");
  
  // Or just use with unbounded enums as long upper bound doesn't exceeds default_unbounded_uuper_range
  ut::expect(simple_enum::enum_name(any_unbounded_enum::value) == "value");
  // default_unbounded_uuper_range can be declared by users before inclusion of simple_enum.hpp
  namespace simple_enum
  {
  #define SIMPLE_ENUM_CUSTOM_UNBOUNDED_RANGE
  inline constexpr auto default_unbounded_uuper_range = 10;
  }
  #include <simple_enum/simple_enum.hpp>
  
```
# performance 
How v0.2.0 compares with Instantiation time:
simple enum for bounded enums to magic_enum::enum_name v0.9.5 and reflect::enum_name main/02.02.2024
clang 17 -ftime-trace on AMD Ryzen 9 5900X Linux desktop 6.6.11-gentoo-dist

<table border="1">
  <tr>
    <th>enum</th>
    <th>framework</th>
    <th>enumerations</th>
    <th>instantiation time</th>
    <th>*code gen</th>
    <th>total time</th>
  </tr>
  <tr>
    <td rowspan="3">lorem_ipsum_long</td>
    <td>simple_enum</td>
    <td rowspan="3">104</td>
    <td>12,235 ms</td>
    <td></td>
    <td>12,235 ms</td>
  </tr>
  <tr>
    <td>magic_enum</td>
    <td>199,867 ms</td>
    <td></td>
    <td>199,867 ms</td>
  </tr>
  <tr>
    <td>reflect</td>
    <td>19,282 ms</td>
    <td>36,470 ms</td>
    <td>55,752 ms</td>
  </tr>
  <tr>
    <td rowspan="3">E</td>
    <td>simple_enum</td>
    <td rowspan="3">15</td>
    <td>2,607 ms</td>
    <td></td>
    <td>2,607 ms</td>
  </tr>
  <tr>
    <td>magic_enum</td>
    <td>86,089 ms</td>
    <td></td>
    <td>86,089 ms</td>
  </tr>
  <tr>
    <td>reflect</td>
    <td>19,091 ms</td>
    <td>27,475 ms</td>
    <td>46,566 ms</td>
  </tr>
  <tr>
    <td rowspan="3">lorem_ipsum_short</td>
    <td>simple_enum</td>
    <td rowspan="3">40</td>
    <td>5,286 ms</td>
    <td></td>
    <td>5,286 ms</td>
  </tr>
  <tr>
    <td>magic_enum</td>
    <td>178,864 ms</td>
    <td></td>
    <td>178,864 ms</td>
  </tr>
  <tr>
    <td>reflect</td>
    <td>20,253 ms</td>
    <td>41,024 ms</td>
    <td>61,277 ms</td>
  </tr>
  <tr>
    <td rowspan="3">strong_untyped</td>
    <td>simple_enum</td>
    <td rowspan="3">3</td>
    <td>0,830 ms</td>
    <td></td>
    <td>0,830 ms</td>
  </tr>
  <tr>
    <td>magic_enum</td>
    <td>151,590 ms</td>
    <td></td>
    <td>151,590 ms</td>
  </tr>
  <tr>
    <td>reflect</td>
    <td>22,732 ms</td>
    <td>46,484 ms</td>
    <td>69,216 ms</td>
  </tr>
</table>





*reflect is not fully comparable as part of calculations are moved to runtime and have to be paid at each enum_name call, so in table included is code generation time which is not present for simple and magic.

screen for simple_enum is from ver 0.0.1 , very early so timing is now much better as in table.
![obraz](https://github.com/arturbac/simple_enum/assets/14975842/b2675b3a-491f-49e5-a507-5693d729e06b)

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/e83a0508-9c31-48e2-b841-90e2233d5a5c)

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/db7cb51b-6ba9-45aa-b5a4-fae5049c30df)



 
