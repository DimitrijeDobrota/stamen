set(GENERATE_OUT "${CMAKE_BINARY_DIR}/bin")

configure_file(demo_menu.conf ${GENERATE_OUT}/demo_menu.conf COPYONLY)

add_custom_command(
    OUTPUT ${GENERATE_OUT}/demo_menu.hpp ${GENERATE_OUT}/demo_menu.cpp
    COMMAND ${GENERATE_OUT}/stamen-generate ${GENERATE_OUT}/demo_menu.conf
    DEPENDS demo_menu.conf stamen-generate
    COMMENT "Generating menu files"
)

add_executable(demo
    main.cpp
    ${GENERATE_OUT}/demo_menu.cpp
)

target_link_libraries(demo stamen)
target_include_directories(demo PRIVATE ${GENERATE_OUT} ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${GENERATE_OUT}"
)



add_custom_command(
    OUTPUT ${GENERATE_OUT}/demo_menu.h ${GENERATE_OUT}/demo_menu.c
    COMMAND ${GENERATE_OUT}/stamen-generate ${GENERATE_OUT}/demo_menu.conf c
    DEPENDS demo_menu.conf stamen-generate
    COMMENT "Generating cmenu files"
)

add_executable(cdemo
    main.c
    ${GENERATE_OUT}/demo_menu.c
)

target_link_libraries(cdemo stamen)
target_include_directories(cdemo PRIVATE ${GENERATE_OUT} ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(cdemo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${GENERATE_OUT}"
)

add_executable(dynamic dynamic.cpp)
target_link_libraries(dynamic stamen)

set_target_properties(dynamic PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
