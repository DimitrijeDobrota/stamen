add_executable(demo
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.cpp
)

target_link_libraries(demo
    stamen
)

ADD_CUSTOM_COMMAND(
    OUTPUT demo_menu.h demo_menu.cpp
    COMMAND ${CMAKE_BINARY_DIR}/bin/stamen-generate ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.conf
    DEPENDS demo_menu.conf stamen-generate
    COMMENT "Generating menu files"
)

ADD_CUSTOM_TARGET(GenerateMenu
    DEPENDS demo_menu.h demo_menu.cpp
    COMMENT "Check if re-generation of menu files is required"
)

ADD_DEPENDENCIES(demo GenerateMenu)

set_target_properties(demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

CONFIGURE_FILE(demo_menu.conf ${CMAKE_BINARY_DIR}/bin/demo_menu.conf  COPYONLY)
