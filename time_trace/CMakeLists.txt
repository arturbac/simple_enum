add_executable(simple_enum_tt)
target_sources(simple_enum_tt PRIVATE simple_enum.cc )
target_link_libraries(simple_enum_tt PRIVATE simple_enum::simple_enum)

add_executable(magic_enum_tt)
target_sources(magic_enum_tt PRIVATE magic_enum.cc )
target_link_libraries(magic_enum_tt PRIVATE magic_enum::magic_enum)

add_executable(reflect_tt ) # EXCLUDE_FROM_ALL
target_sources(reflect_tt PRIVATE reflect.cc )
target_link_libraries(reflect_tt PRIVATE reflect::reflect)
