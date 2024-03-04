#include <simple_enum/enum_cast.hpp>
#include "enum_definitions.h"

using simple_enum::enum_cast;
using simple_enum::enum_name;

int main()
  {
  (void)enum_cast<lorem_ipsum_long>("exercitation");
  (void)enum_cast<lorem_ipsum_short>("exercitation");
  (void)enum_cast<E>("_11");
  (void)enum_cast<strong_untyped>("v3");
  // for veryfication should reuse same instantations of enum_meta_info_t
  (void)enum_name(lorem_ipsum_long::exercitation);
  (void)enum_name(E::_1);
  (void)enum_name(lorem_ipsum_short::exercitation);
  (void)enum_name(strong_untyped::v1);
  }

