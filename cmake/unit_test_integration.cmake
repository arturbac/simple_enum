include( ${CMAKE_CURRENT_LIST_DIR}/default_clang_warnings.cmake)

function(add_ut_test source_file_name)

  get_filename_component(test_executable_name ${source_file_name} NAME_WE)
  add_executable(${test_executable_name} ${source_file_name})
  target_link_libraries(${test_executable_name} PRIVATE simple_enum::simple_enum Boost::ut )
  target_compile_options(${test_executable_name} PUBLIC -Wno-global-constructors )
  add_test(NAME "${test_executable_name}"
           COMMAND ${test_executable_name})

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    target_compile_options( ${test_executable_name} PRIVATE ${clang_default_warnings} )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options( ${test_executable_name} PRIVATE -Wall -Wextra )
  endif()

endfunction()
