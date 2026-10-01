# `std::format` Support for enumerations

## Overview

Starting from C++20, `std::format` introduces a new, type-safe way to perform string formatting. The `simple_enum` library extends `std::format` support to enumerations through a custom `std::formatter` specialization, enabling seamless string formatting of enum values.

## Availability

This feature is available under the following conditions:

- The C++20 standard is being used, or the `__cpp_lib_format` feature test macro is defined, indicating the availability of `std::format`.

## Implementation

The custom `std::formatter` specialization for enumerations is provided through the `simple_enum/std_format.hpp` header.

- The formatter is defined only for enumerations satisfying `simple_enum::bounded_enum` (`first`/`last` enumerators, `simple_enum::info` specialization or `adl_enum_bounds`). It is not defined for other enumerations, so standard enumerations like `std::byte` or `std::errc` are not affected.
- The format spec is the same as for `std::string_view` (fill, align, width, precision), e.g. `std::format("{:>10}", value)`.
- A value without a name is formatted as its underlying integer value, e.g. `static_cast<my_enum>(42)` gives `42`.

## Usage

Include the `simple_enum/std_format.hpp` header to enable `std::format` support for your enumerations:

```cpp
#include <simple_enum/std_format.hpp>
```

### Example Code

```cpp
#include <iostream>
#include <format>
#include "simple_enum/std_format.hpp"  // Include custom formatter support

enum class lorem_ipsum_short { eu, occaecat, dolore, first = eu, last = dolore };

int main() {
    // Use std::format to format an enum value directly
    std::cout << std::format("{}\n", lorem_ipsum_short::eu);  // Outputs: eu
    std::cout << std::format("[{:>8}]\n", lorem_ipsum_short::dolore);  // Outputs: [  dolore]
    std::cout << std::format("{}\n", static_cast<lorem_ipsum_short>(42));  // Outputs: 42
}
```

This code snippet demonstrates how to format an enumeration value to a string using `std::format`, provided that the enum satisfies `simple_enum::bounded_enum` and `simple_enum/std_format.hpp` is included.

## Requirements

- Compiler and library support for C++20 and specifically for the `std::format` feature.
