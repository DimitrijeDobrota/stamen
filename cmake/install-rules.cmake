if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/stamen-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package stamen)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT stamen_Development
)

install(
    TARGETS stamen_stamen
    EXPORT stamenTargets
    RUNTIME #
    COMPONENT stamen_Runtime
    LIBRARY #
    COMPONENT stamen_Runtime
    NAMELINK_COMPONENT stamen_Development
    ARCHIVE #
    COMPONENT stamen_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    stamen_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE stamen_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(stamen_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${stamen_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT stamen_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${stamen_INSTALL_CMAKEDIR}"
    COMPONENT stamen_Development
)

install(
    EXPORT stamenTargets
    NAMESPACE stamen::
    DESTINATION "${stamen_INSTALL_CMAKEDIR}"
    COMPONENT stamen_Development
)

install(
	TARGETS stamen_exe
    RUNTIME COMPONENT stamen_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
