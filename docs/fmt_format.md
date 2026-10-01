# `fmt::format` Support for Enumerations

## Overview

`fmt::format` is a part of the {fmt} library, a modern formatting library for C++. The `simple_enum` library extends `fmt::format` support to enumerations via a custom formatter specialization, enabling easy and efficient string formatting of enum values using the {fmt} library.

## Implementation

To enable `fmt::format` support for your enumeration types, include the custom formatter specialization provided in `simple_enum/fmtlib_format.hpp`.

### Custom Formatter

The `simple_enum/fmtlib_format.hpp` header defines a `fmt::formatter` specialization for enumeration types, leveraging `simple_enum::enum_name` for converting enum values to their string representations.

```cpp
#include <fmt/format.h>

template<typename T>
  requires simple_enum::bounded_enum<T>
struct fmt::formatter<T> : fmt::formatter<fmt::string_view> {
  // Formatting logic here
};
```

- The formatter is defined only for enumerations satisfying `simple_enum::bounded_enum` (`first`/`last` enumerators, `simple_enum::info` specialization or `adl_enum_bounds`). Other enumerations, like `std::byte`, keep formatting provided by {fmt}.
- The format spec is the same as for `fmt::string_view` (fill, align, width, precision), e.g. `fmt::format("{:>10}", value)`.
- A value without a name is formatted as its underlying integer value, e.g. `static_cast<my_enum>(42)` gives `42`.

## Usage

Include the `simple_enum/fmtlib_format.hpp` in your project to use `fmt::format` with enumeration types:

```cpp
#include <simple_enum/fmtlib_format.hpp>
```

### Example Code

```cpp
#include <iostream>
#include <simple_enum/fmtlib_format.hpp>  // Include custom formatter support
#include <fmt/format.h>

enum class lorem_ipsum_short { eu, occaecat, dolore, first = eu, last = dolore };

int main() {
    // Use fmt::format to format an enum value directly
    std::cout << fmt::format("{}\n", lorem_ipsum_short::eu);  // Outputs: eu
    std::cout << fmt::format("[{:>8}]\n", lorem_ipsum_short::dolore);  // Outputs: [  dolore]
    std::cout << fmt::format("{}\n", static_cast<lorem_ipsum_short>(42));  // Outputs: 42
}
```

This example demonstrates using `fmt::format` to format an enumeration value directly to a string, provided the enumeration satisfies `simple_enum::bounded_enum` and the custom formatter is included.

## Requirements

- The {fmt} library must be included in your project.
- Enumerations must satisfy `simple_enum::bounded_enum`.

Integrating `fmt::format` support for enums enhances the flexibility and expressiveness of formatting operations in C++ applications, making it easier to incorporate enum values into formatted strings with the efficiency and type safety of the {fmt} library.
