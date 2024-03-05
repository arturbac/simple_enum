# `enum_name_t`

## Overview

The `enum_name_t` struct provides a mechanism to convert enumeration values to their string names efficiently at compile-time or runtime, leveraging pre-defined metadata.

## Interface Declaration

```cpp
template<enum_concept enum_type>
constexpr auto operator()(enum_type value) const noexcept -> std::string_view;
```

- **Parameters**:
  - `enum_type value`: The enumeration value to convert to a string name.
- **Returns**: 
  - A `std::string_view` representing the name of the enumeration value if within the defined range; otherwise, returns an empty string view.

## Usage Example

Given an enumeration `my_enum` with defined bounds, you can convert an enum value to its string name as follows:

```cpp
enum class my_enum { value1, value2, value3 };

// Assuming bounds are defined for `my_enum` using one of 3 methods example below using adl `adl_enum_bounds`
consteval auto adl_enum_bounds(my_enum) -> simple_enum::adl_info<my_enum> {
    return {my_enum::value1, my_enum::value3};
}

// Usage
std::cout << enum_name(my_enum::value2); // Outputs the string representation of `value2`
```
