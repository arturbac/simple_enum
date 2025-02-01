add_library(
  simple_enum::simple_enum
  ALIAS
  simple_enum)

# ----------------------------------------------------------------
# boost-ext/ut
# ----------------------------------------------------------------
cpmaddpackage(
  ut
  GITHUB_REPOSITORY
  arturbac/ut-ext
  GIT_TAG
  v2.0.1_7)
find_package(ut-ext REQUIRED)
if(SIMPLE_ENUM_ENABLE_TIME_TRACE
   AND (CMAKE_CXX_COMPILER_ID
        MATCHES
        "Clang"
        OR CMAKE_CXX_COMPILER_ID
           MATCHES
           "AppleClang"
       ))
  # external dependencies only for comparison benchmarking
  cpmaddpackage(
    NAME
    magic_enum
    GITHUB_REPOSITORY
    Neargye/magic_enum
    GIT_TAG
    v0.9.5
    GIT_SHALLOW
    TRUE)
  find_package(magic_enum REQUIRED)

  cpmaddpackage(
    Name
    reflect
    GITHUB_REPOSITORY
    boost-ext/reflect
    GIT_TAG
    main
    GIT_SHALLOW
    TRUE)

  add_library(reflect INTERFACE)
  target_include_directories(reflect SYSTEM INTERFACE ${reflect_SOURCE_DIR})
  target_compile_definitions(reflect INTERFACE REFLECT_DISABLE_STATIC_ASSERT_TESTS)
  add_library(
    reflect::reflect
    ALIAS
    reflect)
  find_package(reflect REQUIRED)
endif()

# ----------------------------------------------------------------
# glaze
# ----------------------------------------------------------------

# set(GLAZE_GIT_TAG "v4.2.2")
if(NOT
   DEFINED
   GLAZE_GIT_TAG)
  set(GLAZE_GIT_TAG "main")
endif()

if(NOT
   TARGET
   glaze::glaze)

  cpmaddpackage(
    glaze
    GITHUB_REPOSITORY
    stephenberry/glaze
    GIT_TAG
    ${GLAZE_GIT_TAG}
    GIT_SHALLOW
    TRUE)
endif()

add_ut_test(test_simple_enum.cc)
target_compile_definitions(test_simple_enum PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(ranges_views_ut.cc)
target_compile_definitions(ranges_views_ut PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(enum_cast_ut.cc)
target_compile_definitions(enum_cast_ut PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(enum_index_ut.cc)
target_compile_definitions(enum_index_ut PRIVATE BOOST_UT_DISABLE_MODULE)

add_ut_test(generic_error_category_ut.cc)
target_compile_definitions(generic_error_category_ut PRIVATE BOOST_UT_DISABLE_MODULE)

if((CMAKE_CXX_COMPILER_ID
    STREQUAL
    "GNU"
    AND CMAKE_CXX_COMPILER_VERSION
        VERSION_GREATER_EQUAL
        "13"
   )
   OR (CMAKE_CXX_COMPILER_ID
       STREQUAL
       "Clang"
       AND CMAKE_CXX_COMPILER_VERSION
           VERSION_GREATER_EQUAL
           "16"
      )
   OR (CMAKE_CXX_COMPILER_ID
       STREQUAL
       "MSVC"))
  add_ut_test(std_format_ut.cc)
endif()

cpmaddpackage(
  NAME
  fmt
  GITHUB_REPOSITORY
  fmtlib/fmt
  GIT_TAG
  11.1.2)

# find_package(fmt REQUIRED)
add_ut_test(fmtlib_format_ut.cc)
target_link_libraries(fmtlib_format_ut PRIVATE fmt::fmt)

if(NOT MSVC)
  add_ut_test(glaze_enum_name_ut.cc)
  target_link_libraries(glaze_enum_name_ut PRIVATE glaze::glaze)
  # glaze causes objects to grow over 4GB on msvc
  # https://developercommunity.visualstudio.com/t/Memory-Explosion:-compiler-limit:-objec/10795558
  # target_compile_options(glaze_enum_name_ut PRIVATE "/bigobj" "/Zm500"  # Increased memory allocation "/Os"     #
  # Favor small code size ) target_link_options(glaze_enum_name_ut PRIVATE "/LARGEADDRESSAWARE")
endif()

add_executable(diagnostics EXCLUDE_FROM_ALL)
target_sources(diagnostics PRIVATE diagnostics.cc)
target_compile_definitions(diagnostics PRIVATE BOOST_UT_DISABLE_MODULE)
