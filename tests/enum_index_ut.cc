#include <simple_enum/enum_index.hpp>
#include "simple_enum_tests.hpp"

using simple_enum::enum_index;
using simple_enum::enum_index_error;

int main()
  {
  "enum_index valid"_test = []
  {
    using simple_enum::to_underlying;
    // Test for valid enum values
    expect(eq(enum_index(lorem_ipsum_short::eu).value(), to_underlying(lorem_ipsum_short::eu)));
    expect(eq(enum_index(lorem_ipsum_short::adipiscing).value(), to_underlying(lorem_ipsum_short::adipiscing)));
    expect(eq(enum_index(lorem_ipsum_short::exercitation).value(), to_underlying(lorem_ipsum_short::exercitation)));
  };

  "enum_index out_of_range"_test = []
  {
    using simple_enum::to_underlying;
    // Assuming there's a way to construct an out-of-range value, e.g., using static_cast
    auto out_of_range_value = static_cast<lorem_ipsum_short>(to_underlying(lorem_ipsum_short::last) + 1);
    expect(enum_index(out_of_range_value).error() == enum_index_error::out_of_range);

    // If you have a specific mechanism to test out-of-range values, adjust the above accordingly.
  };
  }

