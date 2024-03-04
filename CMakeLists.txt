cmake_minimum_required(VERSION 3.27)
project(acerola_jam0)

find_package(raylib REQUIRED)
find_package(LDtkLoader 1.5)


set(CMAKE_CXX_STANDARD 20)

add_executable(acerola_jam0 main.cpp
        includes/components.h
        includes/levels.cpp
        includes/levels.h
        includes/factories.cpp
        includes/factories.h
        includes/constants.h
        includes/rendering.cpp
        includes/rendering.h
        includes/collisions.cpp
        includes/collisions.h
        includes/controls.cpp
        includes/controls.h
        includes/npc.cpp
        includes/npc.h
        includes/raygui.h)

target_link_libraries(acerola_jam0 PRIVATE raylib)
target_link_libraries(acerola_jam0 PRIVATE LDtkLoader)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/current/")
