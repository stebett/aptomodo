cmake_minimum_required(VERSION 3.27)
project(acerola_jam0)

find_package(raylib REQUIRED)


set(CMAKE_CXX_STANDARD 20)

add_executable(acerola_jam0 main.cpp
        includes/components.h)
target_link_libraries(acerola_jam0 PRIVATE raylib)
