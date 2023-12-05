add_library(stamen-include INTERFACE)
target_include_directories(stamen-include INTERFACE ../include)

add_library(stamen stamen.cpp menu.cpp)
target_link_libraries(stamen PUBLIC stamen-include)
set_target_properties(stamen PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ../include/stamen.h
)

install(TARGETS stamen
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

add_executable(stamen-generate generate.cpp)
target_link_libraries(stamen-generate PRIVATE stamen)
target_include_directories(stamen-generate PUBLIC ../include)

set_target_properties(stamen-generate PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS stamen-generate DESTINATION bin)
