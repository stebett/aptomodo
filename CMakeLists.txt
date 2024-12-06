if (EMSCRIPTEN)
    set(CMAKE_TOOLCHAIN_FILE /home/ginko/.local/src/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s STACK_SIZE=165536  -s ALLOW_MEMORY_GROWTH")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s NO_DISABLE_EXCEPTION_CATCHING ")
endif ()
cmake_minimum_required(VERSION 3.27)
project(aptomodo)
set(CMAKE_CXX_STANDARD 20)
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()


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
add_subdirectory(vendors/rapidcsv)
add_subdirectory(vendors/box2d)
#add_subdirectory(vendors/spdlog)

set(EXEC game)
set(INCLUDE "includes/")
include_directories(${INCLUDE})

file(GLOB_RECURSE SOURCES ${INCLUDE}/*.cpp)


add_executable(${EXEC} main.cpp ${SOURCES})
target_link_libraries(${EXEC} PRIVATE raylib)
target_link_libraries(${EXEC} PRIVATE box2d)
target_link_libraries(${EXEC} PRIVATE LDtkLoader)
target_link_libraries(${EXEC} PRIVATE EnTT::EnTT)
target_link_libraries(${EXEC} PRIVATE rlimgui)
target_link_libraries(${EXEC} PRIVATE imgui)
target_link_libraries(${EXEC} PRIVATE rapidcsv)
#target_link_libraries(${EXEC} PRIVATE spdlog)
target_link_libraries(${EXEC} PRIVATE tomlplusplus::tomlplusplus)

target_compile_definitions(${EXEC} PUBLIC ASSETS_PATH="assets/current/")
target_compile_definitions(${EXEC} PUBLIC CONFIG_PATH="config/")
target_compile_definitions(${EXEC} PUBLIC AUDIO_PATH="assets/tracks/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
else ()
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
endif ()

target_precompile_headers(${EXEC} PUBLIC includes/pch.h)
if (EMSCRIPTEN)
    set_target_properties(${EXEC} PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/config/ --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/")
#    set_target_properties(${EXEC} PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/")
    set_target_properties(${EXEC} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config)
    target_compile_definitions(${EXEC} PUBLIC ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
else ()
    target_compile_definitions(${EXEC} PUBLIC ROOT_PATH="../")
endif ()