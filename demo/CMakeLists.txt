add_executable(demo
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.cpp
)

target_link_libraries(demo
    stamen
)

add_custom_command(
    OUTPUT demo_menu.hpp demo_menu.cpp
    COMMAND ${CMAKE_BINARY_DIR}/bin/stamen-generate ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.conf
    DEPENDS demo_menu.conf stamen-generate
    COMMENT "Generating menu files"
)

add_custom_target(GenerateMenu
    DEPENDS demo_menu.hpp demo_menu.cpp
    COMMENT "Check if re-generation of menu files is required"
)

add_dependencies(demo GenerateMenu)

set_target_properties(demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

configure_file(demo_menu.conf ${CMAKE_BINARY_DIR}/bin/demo_menu.conf COPYONLY)

add_executable(cdemo
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.c
)

target_link_libraries(cdemo
    stamenc
)

add_custom_command(
    OUTPUT demo_menu.h demo_menu.c
    COMMAND ${CMAKE_BINARY_DIR}/bin/stamen-generate ${CMAKE_CURRENT_SOURCE_DIR}/demo_menu.conf c
    DEPENDS demo_menu.conf stamen-generate
    COMMENT "Generating menu files"
)

add_custom_target(GenerateMenuC
    DEPENDS demo_menu.h demo_menu.c
    COMMENT "Check if re-generation of menu files is required"
)

add_dependencies(cdemo GenerateMenuC)

set_target_properties(cdemo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
