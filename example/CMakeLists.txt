cmake_minimum_required(VERSION 3.14)

project(stamenExamples CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
if(PROJECT_IS_TOP_LEVEL)
  find_package(stamen REQUIRED)
endif()

configure_file(demo_menu.conf demo_menu.conf COPYONLY)

add_custom_target(run-examples)

add_custom_command(
  OUTPUT demo_menu.hpp demo_menu.cpp
  COMMAND stamen::exe --namespace example demo_menu.conf
  DEPENDS demo_menu.conf stamen::exe
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating menu files"
)

function(add_example NAME EXT)
  add_executable("${NAME}" "${NAME}.${EXT}")
  target_include_directories("${NAME}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
  target_link_libraries("${NAME}" PRIVATE stamen::stamen)
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target("run_${NAME}" COMMAND "${NAME}" demo_menu.conf VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

add_example(static cpp)
target_sources(static PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/demo_menu.cpp")

add_example(dynamic cpp)

add_folders(Example)
