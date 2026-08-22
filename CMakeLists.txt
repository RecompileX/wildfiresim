cmake_minimum_required(VERSION 3.20)

project(wildfiresim)

add_executable(wildfiresim
    main.cpp
    cellGrid.cpp
)

target_link_libraries(wildfiresim PRIVATE
    raylib
)
