include(CMakeFindDependencyMacro)

find_dependency(cemplate)
find_dependency(poafloc)

if(cemplate_FOUND AND poafloc_FOUND)
  include("${CMAKE_CURRENT_LIST_DIR}/stamenTargets.cmake")
endif()
