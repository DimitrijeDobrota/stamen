add_library(menu
    menu.cpp
    display.cpp
)

target_include_directories(menu
    PUBLIC ../include
)


add_executable(demo
    main.cpp
)

target_link_libraries(demo
    PRIVATE menu
)

set_target_properties(demo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
