cmake_minimum_required(VERSION 3.21)

# target_link_libraries( unitest_core
#   PUBLIC
#     amrpc::core
#     Boost::ut
#     )
# target_include_directories( unitest_core PUBLIC include )
# 
# if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#   target_compile_options(unitest_core
#     PUBLIC
#       -Wno-disabled-macro-expansion
#       -Wno-used-but-marked-unused
#       -Wno-global-constructors
#       -Wno-exit-time-destructors
#       -Wno-ctad-maybe-unsupported
#       )
# endif()
# Find package boost-ext/ut
# find_package(Boost REQUIRED)

add_executable(test_simple_enum test_simple_enum.cpp)
target_link_libraries(test_simple_enum PRIVATE simple_enum Boost::ut)
target_include_directories(test_simple_enum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_test( NAME "test_simple_enum" COMMAND test_simple_enum )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  target_compile_options( test_simple_enum PRIVATE -Weverything -Wno-unsafe-buffer-usage -Wno-pre-c++17-compat
        -Wno-c++98-compat -Wno-pre-c++14-compat -Wno-pre-c++17-compat -Wno-c++98-compat-pedantic
        -Wno-pre-c++20-compat-pedantic -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors
        )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options( test_simple_enum PRIVATE -Wall -Wextra )
endif()
# include(CTest)
# include(Catch)
# catch_discover_tests(test_simple_enum)
