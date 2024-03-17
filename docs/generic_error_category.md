
# `generic_error_category` Template

This documentation explains how to use the `generic_error_category` template from the `simple_enum` library for handling errors with std::error_code and std::expected, utilizing an example from the `custom_error_example` namespace.

## Including Headers

The first step involves including the necessary headers from the `simple_enum` library:

```cpp
#include <simple_enum/generic_error_category.hpp>
// For c++20 excpected is provided as builtin
#include <simple_enum/expected.h>
```

## Declaring Custom Error Enum

Define a custom error enumeration within your namespace for use by functions. This example uses `function_error` as the custom enumeration:

```cpp
namespace custom_error_example 
  {
enum class function_error {
  failed_other_reason,
  unhandled_exaption
  };
```

Following the declaration, define the bounds for your custom enum using the `adl_enum_bounds` function, ensuring the enum is compatible with the `simple_enum` library's functionalities:

```cpp
consteval auto adl_enum_bounds(function_error) 
  {
  using enum function_error;
  return simple_enum::adl_info{failed_other_reason, unhandled_exaption};
  }
}  // namespace custom_error_example
```

## Making the Enum Compatible with `std::is_error_code_enum`

To integrate seamlessly with the C++ error handling mechanisms, specialize the `std::is_error_code_enum` for your custom error enum:

```cpp
template<>
struct std::is_error_code_enum<custom_error_example::function_error> : true_type {
};
```

## Defining Error Category

Within your namespace, define a unique error category by specifying a category name and using the `generic_error_category` template:

```cpp
namespace custom_error_example 
  {
inline constexpr simple_enum::basic_fixed_string category_name{"Function Error Category Name"};
using category_name_literal = simple_enum::string_literal<category_name>;

using function_error_category = simple_enum::generic_error_category<function_error, category_name_literal>;
  }  // namespace custom_error_example
```

Declare the template class to inform compiler that explicit instantiation is provided explicitly:
Note this specialization is defined in namespace simple_enum;

```cpp
using custom_error_example::function_error;
using custom_error_example::category_name_literal;
extern template class simple_enum::generic_error_category<function_error, category_name_literal>;
```

## Implementation in Translation Unit

To ensure there's only a single instantiation of the error category, include the implementation file in single translation unit and declare explicit instantiation:

```cpp
#include <simple_enum/generic_error_category_impl.hpp>

template class simple_enum::generic_error_category<function_error, category_name_literal>;
```

## Using the Custom Error Category

Finally, demonstrate the usage of your custom error category within functions, handling errors gracefully:

```cpp
namespace custom_error_example 
  {
using cxx23::expected;
using cxx23::unexpected;

static auto my_function(int arg) -> expected<void, std::error_code> 
  {
  if(arg != 0)
    return unexpected{function_error_category::make_error_code(function_error::failed_other_reason)};
  return {};
  }
catch(...) 
  {
  return unexpected{function_error_category::make_error_code(function_error::unhandled_exaption)};
  }
  }  // namespace custom_error_example
```

In your application's code, handle the error cases by inspecting the `expected` object returned by functions using your custom error category:

```cpp
int main(int argc, char const * const *) 
  {
  auto res{my_function(argc)};
  if(!res) 
    {
    std::error_code ec{std::move(res.error())};
    fmt::print(
      "Error Category:"{}" message:"{}" enumeration value:"{}"",
      ec.category().name(),
      ec.message(),
      function_error_category::enumeration(ec.value())
      );
    }
  return EXIT_SUCCESS;
  }
```

Full example is available here : [generic_error_category](https://github.com/arturbac/simple_enum/blob/master/examples/generic_error_category.cc)
