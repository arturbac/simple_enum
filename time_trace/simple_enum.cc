#include <simple_enum/simple_enum.hpp>
using simple_enum::enum_name;

#include "enum_definitions.h"
// only for benchmarking comparison 
int main()
{
  (void)enum_name(E::_1);
  (void)enum_name(strong_typed::v1);
  (void)enum_name(strong_untyped::v1);
  (void)enum_name(v1);
  (void)enum_name(test::strong_typed_2_e::v1);
  (void)enum_name(test::strong_untyped_2_e::v1);
  (void)enum_name(test::subnamespace::example_3_e::v1);
  (void)enum_name(test::subnamespace::v1);
  (void)enum_name(test::subnamespace::detail::example_4_e::v1);
  (void)enum_name(test::subnamespace::example_5_e::v1);
  (void)enum_name(test::subnamespace::v1);
  (void)enum_name(lorem_ipsum_short::exercitation);

  (void)enum_name(lorem_ipsum_long::exercitation);
}
