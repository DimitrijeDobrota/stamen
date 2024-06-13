add_library(stamen STATIC stamen.cpp menu.cpp c_bindings.cpp)
target_include_directories(stamen PRIVATE ../include)
target_compile_definitions(stamen PRIVATE WITH_C_BINDINGS)
set_target_properties(stamen PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(stamen PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "d"
    PUBLIC_HEADER "include/stamen.h;include/stamen.hpp"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_executable(stamen-generate generate.cpp)
target_link_libraries(stamen-generate PRIVATE args stamen)
target_include_directories(stamen-generate PRIVATE ../include)

set_target_properties(stamen-generate PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
