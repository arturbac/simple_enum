
add_ut_test(test_simple_enum.cc)
target_compile_definitions( test_simple_enum PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(ranges_views_ut.cc)
target_compile_definitions( ranges_views_ut PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(enum_cast_ut.cc)
target_compile_definitions( enum_cast_ut PRIVATE BOOST_UT_DISABLE_MODULE)

# add_ut_test(enum_index_ut.cc)
# target_compile_definitions( enum_index_ut PRIVATE BOOST_UT_DISABLE_MODULE)

add_executable(diagnostics EXCLUDE_FROM_ALL)
target_sources( diagnostics PRIVATE diagnostics.cc)
target_compile_definitions( diagnostics PRIVATE BOOST_UT_DISABLE_MODULE)
