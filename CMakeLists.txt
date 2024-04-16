cmake_minimum_required(VERSION 3.27)
project(acerola_jam0)
set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)
find_package(LDtkLoader 1.5)

set(sources
        includes/rlImGui/rlImGui.h
        includes/rlImGui/rlImGui.cpp
        includes/imgui/imconfig.h
        includes/imgui/imgui.cpp
        includes/imgui/imgui.h
        includes/imgui/imgui_demo.cpp
        includes/imgui/imgui_draw.cpp
        includes/imgui/imgui_internal.h
        includes/imgui/imgui_tables.cpp
        includes/imgui/imgui_widgets.cpp
        includes/imgui/imstb_rectpack.h
        includes/imgui/imstb_textedit.h
        includes/imgui/imstb_truetype.h
        includes/astar.h
        includes/astar.cpp
        includes/components.h
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
        includes/raygui.h
        includes/gui.h
        includes/gui.cpp
        includes/timer.h
        includes/config.h
        includes/config.cpp
)


add_executable(acerola_jam0 main.cpp ${sources})


target_link_libraries(acerola_jam0 PRIVATE raylib)
target_link_libraries(acerola_jam0 PRIVATE LDtkLoader)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/current/")
