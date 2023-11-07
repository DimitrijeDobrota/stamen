cmake_minimum_required(VERSION 3.27.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    Menu
    VERSION 0.0.1
    DESCRIPTION "Experimentation with dinamic menus"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
