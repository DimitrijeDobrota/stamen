add_library(stamen INTERFACE)

target_include_directories(stamen
    INTERFACE ../include
)

add_library(stamen-display
    stamen.cpp
)

target_link_libraries(stamen-display
    PRIVATE stamen
)

add_executable(stamen-generate generate.cpp)

target_link_libraries(stamen-generate
    PRIVATE stamen
)

target_include_directories(stamen-generate
    PUBLIC ../include
)

set_target_properties(stamen-generate PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
