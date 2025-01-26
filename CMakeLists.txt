cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0167 NEW)
cmake_policy(SET CMP0175 NEW)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/extract_version.cmake)

project(
  simple_enum
  VERSION ${SIMPLE_ENUM_VERSION}
  LANGUAGES CXX
)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()
if(CMAKE_CXX_STANDARD LESS 23)
  message(FATAL_ERROR "Minimum required standard is C++23")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "simple_enum version v${SIMPLE_ENUM_VERSION}")

include(cmake/get_cpm.cmake)

option(SIMPLE_ENUM_USE_GLAZE_3_1 "Use Glaze version 3.1" OFF)
option(SIMPLE_ENUM_USE_GLAZE_4_0 "Use Glaze version 4.2" ON)
option(SIMPLE_ENUM_ENABLE_TESTS "Enable unit tests" ON)
option(SIMPLE_ENUM_EXPORT_CMAKE_TARGETS "Enable cmake targets" ON)

if(SIMPLE_ENUM_USE_GLAZE_3_1)
  set(GLAZE_GIT_TAG "v3.4.3")
elseif(SIMPLE_ENUM_USE_GLAZE_4_0)
  set(GLAZE_GIT_TAG "v4.2.2")
else()
  message(FATAL_ERROR "Please select a Glaze version by setting USE_GLAZE_3_1, or SIMPLE_ENUM_USE_GLAZE_4_0")
endif()

add_library(simple_enum INTERFACE)

if(SIMPLE_ENUM_USE_GLAZE_3_1)
  target_compile_definitions(simple_enum INTERFACE SIMPLE_ENUM_GLZ_3_1_x)
elseif(SIMPLE_ENUM_USE_GLAZE_4_0)
  target_compile_definitions(simple_enum INTERFACE SIMPLE_ENUM_GLZ_3_1_x)
else()
  message(FATAL_ERROR "Future, unimplemented")
endif()
target_compile_features(simple_enum INTERFACE cxx_std_23)

if(SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS)
  target_compile_definitions(simple_enum INTERFACE SIMPLE_ENUM_OPT_IN_STATIC_ASSERTS=1)
endif()

target_sources(
  simple_enum INTERFACE FILE_SET HEADERS BASE_DIRS $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                        $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

install(
  TARGETS simple_enum
  EXPORT simple_enum_targets
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET HEADERS
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(
  cmake/simple_enumConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple_enum
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/presetsConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/simple_enumConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple_enum
)

if(SIMPLE_ENUM_EXPORT_CMAKE_TARGETS)
  install(
    EXPORT simple_enum_targets
    FILE simple_enumTargets.cmake
    NAMESPACE simple_enum::
    DESTINATION lib/cmake/simple_enum
  )
endif()

if(PROJECT_IS_TOP_LEVEL AND SIMPLE_ENUM_ENABLE_TESTS)
  include(cmake/unit_test_integration.cmake)
  # Enable testing and add the tests directory
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(examples)

  if(SIMPLE_ENUM_ENABLE_TIME_TRACE AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang"))
    add_subdirectory(time_trace)
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --list-presets WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} --workflow --list-presets WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
