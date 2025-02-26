cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    stamen
    VERSION 1.2.8
    DESCRIPTION "Static menu generator"
    HOMEPAGE_URL "https://git.dimitrijedobrota.com/stamen"
    LANGUAGES C CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

find_package(poafloc 1.2 CONFIG REQUIRED)
find_package(cemplate 0.2 CONFIG REQUIRED)

# ---- Declare library ----

add_library(
    stamen_stamen
    source/stamen.cpp
)
target_link_libraries(stamen_stamen PUBLIC poafloc::poafloc)
add_library(stamen::stamen ALIAS stamen_stamen)

include(GenerateExportHeader)
generate_export_header(
    stamen_stamen
    BASE_NAME stamen
    EXPORT_FILE_NAME export/stamen/stamen_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(stamen_stamen PUBLIC STAMEN_STATIC_DEFINE)
endif()

set_target_properties(
    stamen_stamen PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME stamen
    OUTPUT_NAME stamen
)

target_include_directories(
    stamen_stamen ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    stamen_stamen SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(stamen_stamen PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(stamen_exe source/generate.cpp)
add_executable(stamen::exe ALIAS stamen_exe)

target_link_libraries(stamen_exe PRIVATE cemplate::cemplate)
target_link_libraries(stamen_exe PRIVATE stamen::stamen)
set_property(TARGET stamen_exe PROPERTY OUTPUT_NAME stamen)

target_compile_features(stamen_exe PRIVATE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${stamen_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT stamen_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of stamen"
  )
endif()

include(cmake/dev-mode.cmake)
