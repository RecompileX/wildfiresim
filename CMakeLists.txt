cmake_minimum_required(VERSION 3.20)

project(wildfiresim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(wildfiresim
    src/main.cpp
    src/cellGrid.cpp
    src/ui.cpp
    resources/fire.rc
)

target_include_directories(wildfiresim PRIVATE
    include
    external
)

target_link_libraries(wildfiresim PRIVATE
    raylib
)

if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(wildfiresim PRIVATE stdc++exp)
endif()
