cmake_minimum_required(VERSION 3.27)
project(acerola_jam0)
set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)
find_package(LDtkLoader 1.5)

set(sources
        vendors/rlImGui/rlImGui.h
        vendors/rlImGui/rlImGui.cpp
        vendors/imgui/imconfig.h
        vendors/imgui/imgui.cpp
        vendors/imgui/imgui.h
        vendors/imgui/imgui_demo.cpp
        vendors/imgui/imgui_draw.cpp
        vendors/imgui/imgui_internal.h
        vendors/imgui/imgui_tables.cpp
        vendors/imgui/imgui_widgets.cpp
        vendors/imgui/imstb_rectpack.h
        vendors/imgui/imstb_textedit.h
        vendors/imgui/imstb_truetype.h
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
        vendors/raygui.h
        includes/gui.h
        includes/gui.cpp
        includes/timer.h
        includes/config.h
        includes/config.cpp
        includes/managers/audioManager.cpp
        includes/managers/audioManager.h
        includes/managers/animationManager.cpp
        includes/managers/animationManager.h
        includes/managers/levelManager.cpp
        includes/managers/levelManager.h
        includes/managers/renderingManager.cpp
        includes/managers/renderingManager.h
)


add_executable(acerola_jam0 main.cpp ${sources})


target_link_libraries(acerola_jam0 PRIVATE raylib)
target_link_libraries(acerola_jam0 PRIVATE LDtkLoader)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/current/")
