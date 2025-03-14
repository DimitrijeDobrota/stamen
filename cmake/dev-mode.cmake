include(cmake/folders.cmake)

add_custom_target(
    run-exe
    COMMAND stamen_exe
    VERBATIM
)
add_dependencies(run-exe stamen_exe)

option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(ENABLE_COVERAGE)
  include(cmake/coverage.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
