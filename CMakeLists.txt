cmake_minimum_required(VERSION 3.27)
project(aptomodo)
set(CMAKE_CXX_STANDARD 20)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(vendors/raylib)
add_subdirectory(vendors/LDtkLoader)
add_subdirectory(vendors/entt)
add_subdirectory(vendors/rlImGui)
add_subdirectory(vendors/imgui)
add_subdirectory(vendors/tomlplusplus)

set(EXEC game)
set(INCLUDE "includes/")
include_directories(${INCLUDE})

file(GLOB_RECURSE SOURCES ${INCLUDE}/*.cpp)



add_executable(${EXEC} main.cpp ${SOURCES}
        includes/math/decay.cpp
        includes/math/decay.h
        includes/shadowCast.cpp
        includes/shadowCast.h
        includes/intGrid.cpp
        includes/intGrid.h
        includes/components.cpp
        includes/enemyType.cpp
        includes/enemyType.h
        includes/commands.cpp
        includes/commands.h
        includes/ai/strategies.cpp
        includes/managers/assets.h)
target_link_libraries(${EXEC} PRIVATE raylib)
target_link_libraries(${EXEC} PRIVATE LDtkLoader)
target_link_libraries(${EXEC} PRIVATE EnTT::EnTT)
target_link_libraries(${EXEC} PRIVATE rlImGui)
target_link_libraries(${EXEC} PRIVATE imgui)
target_link_libraries(${EXEC} PRIVATE tomlplusplus::tomlplusplus)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/container_hash/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/config/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/describe/include)
target_include_directories(${EXEC} PRIVATE vendors/boost/libs/mp11/include)

target_compile_definitions(${EXEC} PUBLIC ASSETS_PATH="../assets/current/")
target_precompile_headers(${EXEC} PRIVATE includes/pch.h)
