cmake_minimum_required(VERSION 3.21)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/extract_version.cmake)
project(simple_enum
   VERSION ${SIMPLE_ENUM_VERSION}
   LANGUAGES CXX
   )

if(PROJECT_IS_TOP_LEVEL)
  message(STATUS "simple_enum version v${SIMPLE_ENUM_VERSION}")

  # Specify the C++ standard
  set(CMAKE_CXX_STANDARD 20)
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
  if( SIMPLE_ENUM_ENABLE_TIME_TRACE AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang"))
    # external dependencies only for comparison benchmarking 
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
    find_package(reflect REQUIRED)
  endif()
endif()

# Header-only library target
add_library(simple_enum INTERFACE)
add_library(simple_enum::simple_enum ALIAS simple_enum )
target_include_directories(simple_enum INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/unit_test_integration.cmake)
  # Enable testing and add the tests directory
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(examples)

  if( SIMPLE_ENUM_ENABLE_TIME_TRACE AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang"))
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
endif()
