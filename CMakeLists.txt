cmake_minimum_required(VERSION 3.28..3.31)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/extract_version.cmake)

project(
  simple_enum
  VERSION ${SIMPLE_ENUM_VERSION}
  LANGUAGES CXX)

if(PROJECT_SOURCE_DIR
   STREQUAL
   PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT
   DEFINED
   CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()
if(CMAKE_CXX_STANDARD
   LESS
   23)
  message(FATAL_ERROR "Minimum required standard is C++23")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "simple_enum version v${SIMPLE_ENUM_VERSION}")

include(cmake/get_cpm.cmake)
option(
  SIMPLE_ENUM_ENABLE_TESTS
  "Enable unit tests"
  ON)
option(
  SIMPLE_ENUM_EXPORT_CMAKE_TARGETS
  "Enable cmake targets"
  ON)

option(
  SIMPLE_ENUM_ENABLE_MODULE
  "Build C++23 module instead of INTERFACE library"
  OFF)

if(SIMPLE_ENUM_ENABLE_MODULE)
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)

  add_library(simple_enum STATIC)
  target_compile_definitions(simple_enum PUBLIC SIMPLE_ENUM_CXX_MODULE)
  target_sources(
    simple_enum
    PRIVATE FILE_SET
            HEADERS
            BASE_DIRS
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC FILE_SET
           ext_support
           TYPE
           HEADERS
           BASE_DIRS
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ext_support>
           $<INSTALL_INTERFACE:include>
    PUBLIC FILE_SET
           CXX_MODULES
           FILES
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/module/simple_enum.cxx>
           $<INSTALL_INTERFACE:include>)

else()
  set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
  add_library(simple_enum INTERFACE)
  target_sources(
    simple_enum
    INTERFACE FILE_SET
              HEADERS
              BASE_DIRS
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
    INTERFACE FILE_SET
              ext_support
              TYPE
              HEADERS
              BASE_DIRS
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ext_support>
              $<INSTALL_INTERFACE:include>)
endif()

set_target_properties(
  simple_enum
  PROPERTIES CXX_STANDARD 23
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS OFF
             CXX_SCAN_FOR_MODULES ${SIMPLE_ENUM_ENABLE_MODULE})

if(SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS)
  target_compile_definitions(simple_enum INTERFACE SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS=1)
endif()

install(
  TARGETS simple_enum
  EXPORT simple_enum_targets
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET
  HEADERS
  FILE_SET
  ext_support
  FILE_SET
  CXX_MODULES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/modules)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ext_support/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(cmake/simple_enumConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple_enum)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple_enum)

if(SIMPLE_ENUM_EXPORT_CMAKE_TARGETS)
  install(
    EXPORT simple_enum_targets
    FILE simple_enumTargets.cmake
    NAMESPACE simple_enum::
    DESTINATION lib/cmake/simple_enum)
endif()

if(PROJECT_IS_TOP_LEVEL AND SIMPLE_ENUM_ENABLE_TESTS)
  include(cmake/unit_test_integration.cmake)
  # Enable testing and add the tests directory
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(examples)

  if(SIMPLE_ENUM_ENABLE_TIME_TRACE
     AND (CMAKE_CXX_COMPILER_ID
          MATCHES
          "Clang"
          OR CMAKE_CXX_COMPILER_ID
             MATCHES
             "AppleClang"
         ))
    add_subdirectory(time_trace)
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --list-presets WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} --workflow --list-presets WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
