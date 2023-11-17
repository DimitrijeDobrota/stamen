add_library(stamen-include INTERFACE)
target_include_directories(stamen-include INTERFACE ../include)

add_executable(stamen-generate generate.cpp)
target_link_libraries(stamen-generate PRIVATE stamen-include)
target_include_directories(stamen-generate PUBLIC ../include)

set_target_properties(stamen-generate PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_library(stamen stamen.cpp)
target_link_libraries(stamen PUBLIC stamen-include)
set_target_properties(stamen PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ../include/stamen.h
)

install(TARGETS stamen-generate DESTINATION bin)
install(TARGETS stamen
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
