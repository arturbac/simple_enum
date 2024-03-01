cmake_minimum_required(VERSION 3.21) # Use the latest CMake version you have confirmed supports all features you need, 3.21 as a placeholder
project(simple_enum VERSION 0.0.1 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)
#----------------------------------------------------------------
# boost-ext/ut
#----------------------------------------------------------------
CPMAddPackage(
  ut
  GITHUB_REPOSITORY boost-ext/ut
  GIT_TAG        v2.0.1
)
find_package(ut REQUIRED)
# target_precompile_headers( ut INTERFACE "${ut_SOURCE_DIR}/include/boost/ut.hpp" )
CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.5
)
find_package(magic_enum REQUIRED)

CPMAddPackage(
  Name reflect
  GITHUB_REPOSITORY boost-ext/reflect
  GIT_TAG main
)
add_library(reflect INTERFACE)
target_include_directories(reflect SYSTEM INTERFACE ${reflect_SOURCE_DIR})
target_compile_definitions(reflect INTERFACE REFLECT_DISABLE_STATIC_ASSERT_TESTS)
add_library(reflect::reflect ALIAS reflect)
# target_precompile_headers( reflect INTERFACE "${reflect_SOURCE_DIR}/reflect" )
find_package(reflect REQUIRED)

# Header-only library target
add_library(simple_enum INTERFACE)
add_library(simple_enum::simple_enum ALIAS simple_enum )
target_include_directories(simple_enum INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable testing and add the tests directory
enable_testing()
add_subdirectory(tests)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  add_subdirectory(time_trace)
endif()
execute_process(
  COMMAND ${CMAKE_COMMAND} --list-presets 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
execute_process(
  COMMAND ${CMAKE_COMMAND} --workflow --list-presets 
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
