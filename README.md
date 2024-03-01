# simple_enum
Prof of concept for simple enum_name that bounded enums can be used with enum_name with short instantiation and cost can be proportional
to number of enumerations in enum

There is no runtime cost at all with getting enum_name for runtime variable, the same as in magic_enum.
But computation at compile time is limitted to number of elements in a range thanks to bounded_enum concept which requires from users to declare first,last in enum.
There are otpmisitations related to  enum name substraction so one loop is avoided, and after first enumeration parsed rest of enumerations are  parsed with reduced string literal range.

How v0.0.1 compares with Instantiation time:
simple enum for bounded enums to magic_enum::enum_name and reflect::enum_name


enum | framework   | enumerations | instantiation time | *code gen
-----|------------------------------|-------------:|-------------------:|-------------------:
lorem_ipsum_long | simple_enum                   | 104          | 20,973 ms          | 
lorem_ipsum_long | magic_enum                    | 104          | 199,867 ms         | 
lorem_ipsum_long | reflect                       | 104          | 19,282 ms          | 16,134 + 20,336 ms
E | simple_enum                  | 15           | 3,993 ms           | 
E | magic_enum                   | 15           | 86,089 ms          | 
E | reflect                      | 15           | 19,091 ms          | 12,254 + 15,221 ms
lorem_ipsum_short | simple_enum                   | 40           | 8,694 ms           | 
lorem_ipsum_short | magic_enum                    | 40           | 178,864 ms         | 
lorem_ipsum_short | reflect                       | 40           | 20,253 ms          | 18,016 + 23,008 ms
strong_untyped | simple_enum                   | 3            | 1,120 ms           | 
strong_untyped | magic_enum                    | 3            | 151,590 ms         |
strong_untyped | reflect                       | 3            | 22,732 ms          | 20,171 + 26,313 ms


*reflect is not fully comparable as part of calculations are moved to runtime and have to be paid at each enum_name call, so included is code generation time which is not present for simple and magic

![obraz](https://github.com/arturbac/simple_enum/assets/14975842/b2675b3a-491f-49e5-a507-5693d729e06b)

 
