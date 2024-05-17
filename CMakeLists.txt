cmake_minimum_required(VERSION 3.27)
project(acerola_jam0)
set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)
#find_package(LDtkLoader 1.5)

set(Boost_ROOT "vendors/boost")  # Adjust this path to where you extracted Boost
set(rlImGui_ROOT "vendors/rlImGui")
set(imgui_ROOT "vendors/imgui")
set(entt_ROOT "vendors/entt/src/")
set(eigen_ROOT "vendors/eigen/")
set(LDTKLOADER_ROOT "vendors/LDtkLoader")

set(INCLUDE "includes/")

include_directories(${Boost_ROOT})
include_directories(${rlImGui_ROOT})
include_directories(${imgui_ROOT})
include_directories(${entt_ROOT})
include_directories(${eigen_ROOT})
include_directories(${INCLUDE})

add_subdirectory(${LDTKLOADER_ROOT})

#file(GLOB LDTKLOADER_SOURCES ${LDTKLOADER_ROOT}/src/*.cpp)
file(GLOB rlImGui_SOURCES ${rlImGui_ROOT}/*.cpp)
file(GLOB imgui_SOURCES ${imgui_ROOT}/*.cpp)
file(GLOB_RECURSE INCLUDE_SOURCES ${INCLUDE}/*.cpp)

set(SOURCES
        ${LDTKLOADER_SOURCES}
        ${imgui_SOURCES}
        ${rlImGui_SOURCES}
        ${INCLUDE_SOURCES}
        vendors/raygui.h
)


add_executable(acerola_jam0 main.cpp ${SOURCES})

target_link_libraries(acerola_jam0 PRIVATE raylib)
target_link_libraries(acerola_jam0 PRIVATE LDtkLoader)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/current/")
