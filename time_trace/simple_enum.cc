#include <simple_enum/simple_enum.hpp>
using simple_enum::enum_name;

#include "enum_definitions.h"

// only for benchmarking comparison
int main()
  {
  (void)enum_name(lorem_ipsum_long::exercitation);
  (void)enum_name(E::_1);
  (void)enum_name(lorem_ipsum_short::exercitation);
  (void)enum_name(strong_untyped::v1);
  }
