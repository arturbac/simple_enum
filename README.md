# simple_enum

# features

```cpp
template<bounded_enum enum_type>
constexpr auto enum_name(enum_type value) noexcept -> std::string_view;

template<typename type>
concept enum_concept = std::is_enum_v<type>;

template<typename T>
concept bounded_enum = requires(T e) 
{
  requires enum_concept<T>;
  // require 'first' and 'last' in enum
  { T::first } -> std::convertible_to<T>;
  { T::last } -> std::convertible_to<T>;
  // 'last' must be greater than or equal to 'first'
  requires (to_underlying(T::last) >= to_underlying(T::first));
};

template<typename Enum>
constexpr auto to_underlying(Enum e) noexcept -> std::underlying_type_t<Enum>
 { return static_cast<std::underlying_type_t<Enum>>(e); }
```

Prof of concept for simple enum_name that bounded enums can be used with enum_name with short instantiation and cost can be proportional
to number of enumerations in enum

There is no runtime cost at all with getting enum_name for runtime variable, the same as in magic_enum.
But computation at compile time is limitted to number of elements in a range thanks to bounded_enum concept which requires from users to declare first,last in enum.
There are otpmisitations related to  enum name substraction so one loop is avoided, and after first enumeration parsed rest of enumerations are  parsed with reduced string literal range.

How v0.0.1 compares with Instantiation time:
simple enum for bounded enums to magic_enum::enum_name and reflect::enum_name
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
    <td>16,957 ms</td>
    <td></td>
    <td>16,957 ms</td>
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
    <td>3,397 ms</td>
    <td></td>
    <td>3,397 ms</td>
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
    <td>7,165 ms</td>
    <td></td>
    <td>7,165 ms</td>
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
    <td>1,048 ms</td>
    <td></td>
    <td>1,048 ms</td>
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





*reflect is not fully comparable as part of calculations are moved to runtime and have to be paid at each enum_name call, so in table included is code generation time which is not present for simple and magic

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/b2675b3a-491f-49e5-a507-5693d729e06b)

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/e83a0508-9c31-48e2-b841-90e2233d5a5c)

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/db7cb51b-6ba9-45aa-b5a4-fae5049c30df)



 
