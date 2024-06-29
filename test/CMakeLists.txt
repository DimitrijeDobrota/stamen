cmake_minimum_required(VERSION 3.14)

project(stamenTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(stamen REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(stamen_test source/stamen_test.cpp)
target_link_libraries(stamen_test PRIVATE stamen::stamen)
target_compile_features(stamen_test PRIVATE cxx_std_20)

add_test(NAME stamen_test COMMAND stamen_test)

# ---- End-of-file commands ----

add_folders(Test)
