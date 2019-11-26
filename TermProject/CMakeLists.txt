cmake_minimum_required(VERSION 3.14)
project(TermProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ASCII.hpp
    syntax_tree_node.hpp
    string_pos.hpp
)