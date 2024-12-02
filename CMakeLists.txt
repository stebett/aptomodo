cmake_minimum_required(VERSION 3.27)
project(aptomodo)
set(CMAKE_CXX_STANDARD 20)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(TEMPLATE_DIR ${CMAKE_SOURCE_DIR}/cmake-templates/)
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendors/imgui/CMakeLists.txt")
    file(RENAME ${TEMPLATE_DIR}/custom_imgui_CMakeLists.txt  # custom template file name
            ${CMAKE_SOURCE_DIR}/vendors/imgui/CMakeLists.txt)
    message(STATUS "Copied custom_imgui_CMakeLists.txt for imgui")
endif ()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendors/rlImGui/CMakeLists.txt")
    file(RENAME ${TEMPLATE_DIR}/custom_rlimgui_CMakeLists.txt  # custom template file name
            ${CMAKE_SOURCE_DIR}/vendors/rlImGui/CMakeLists.txt)
    message(STATUS "Copied custom_rlimgui_CMakeLists.txt for rlimgui")
endif ()


add_subdirectory(vendors/raylib)
add_subdirectory(vendors/LDtkLoader)
add_subdirectory(vendors/entt)
add_subdirectory(vendors/rlImGui)
add_subdirectory(vendors/imgui)
add_subdirectory(vendors/tomlplusplus)
add_subdirectory(vendors/box2d)
add_subdirectory(vendors/spdlog)

set(EXEC game)
set(INCLUDE "includes/")
include_directories(${INCLUDE})

file(GLOB_RECURSE SOURCES ${INCLUDE}/*.cpp)


add_executable(${EXEC} main.cpp ${SOURCES}
        includes/systems/inputs.cpp
        includes/systems/statusUpdate.h
        includes/status.h
        includes/attacks.h
        includes/math/bezier.h
        includes/math/bezierEditor.h
        includes/managers/logging.h)
target_link_libraries(${EXEC} PRIVATE raylib)
target_link_libraries(${EXEC} PRIVATE box2d)
target_link_libraries(${EXEC} PRIVATE LDtkLoader)
target_link_libraries(${EXEC} PRIVATE EnTT::EnTT)
target_link_libraries(${EXEC} PRIVATE rlimgui)
target_link_libraries(${EXEC} PRIVATE imgui)
target_link_libraries(${EXEC} PRIVATE spdlog)
target_link_libraries(${EXEC} PRIVATE tomlplusplus::tomlplusplus)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/container_hash/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/config/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/describe/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/mp11/include)

target_compile_definitions(${EXEC} PUBLIC ASSETS_PATH="../assets/current/")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
else ()
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
endif ()
target_precompile_headers(${EXEC} PRIVATE includes/pch.h)
