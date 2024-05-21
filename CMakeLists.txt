cmake_minimum_required(VERSION 3.27)
project(aptomodo)
set(CMAKE_CXX_STANDARD 20)



find_package(raylib REQUIRED)
#find_package(LDtkLoader 1.5)
set(EXEC game)
set(Boost_ROOT "vendors/boost")  # Adjust this path to where you extracted Boost
set(rlImGui_ROOT "vendors/rlImGui")
set(imgui_ROOT "vendors/imgui")
set(entt_ROOT "vendors/entt/src/")
set(eigen_ROOT "vendors/eigen/")
set(LDTKLOADER_ROOT "vendors/LDtkLoader")
set(TOML_ROOT "vendors/toml++/include")
set(INCLUDE "includes/")

include_directories(${Boost_ROOT})
include_directories(${rlImGui_ROOT})
include_directories(${imgui_ROOT})
include_directories(${entt_ROOT})
include_directories(${TOML_ROOT})
include_directories(${eigen_ROOT})
include_directories(${INCLUDE})

add_subdirectory(${LDTKLOADER_ROOT})

file(GLOB rlImGui_SOURCES ${rlImGui_ROOT}/*.cpp)
file(GLOB imgui_SOURCES ${imgui_ROOT}/*.cpp)
file(GLOB_RECURSE INCLUDE_SOURCES ${INCLUDE}/*.cpp)

set(SOURCES
        ${LDTKLOADER_SOURCES}
        ${imgui_SOURCES}
        ${rlImGui_SOURCES}
        ${INCLUDE_SOURCES}
        vendors/raygui.h
        includes/managers/parameters.cpp
        includes/managers/gui.cpp
)


add_executable(${EXEC} main.cpp ${SOURCES})

target_link_libraries(${EXEC} PRIVATE raylib)
target_link_libraries(${EXEC} PRIVATE LDtkLoader)

target_compile_definitions(${EXEC} PUBLIC ASSETS_PATH="assets/current/")


#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/current/")
# Post-build step to copy raylib.dll to the build directory

# Post-build step to copy assets to the build directory
#add_custom_command(TARGET ${EXEC} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/assets
#        $<TARGET_FILE_DIR:${EXEC}>/assets)
#
#add_custom_command(TARGET ${EXEC} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${raylib_LIBRARY}
#        $<TARGET_FILE_DIR:${EXEC}>)
#

## Pre-build
#add_executable(generate config/generate_header.cpp)
#set(TOML_ROOT "vendors/toml++/include")
#target_include_directories(generate PUBLIC ${TOML_ROOT})
#
#add_custom_command(
#        OUTPUT ${CMAKE_SOURCE_DIR}/config/attributes.h
#        COMMAND generate
#        DEPENDS generate ${CMAKE_SOURCE_DIR}/config/attributes.toml
#)
