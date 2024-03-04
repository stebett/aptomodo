#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/levels.h"
#include "includes/rendering.h"
#define RAYGUI_IMPLEMENTATION

#include "includes/raygui.h"

int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");
    auto scene = new GameScene(registry);

    SetTargetFPS(60);
//    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
//    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiLoadStyle(getAssetPath("gui_style.rgs").c_str());

    auto rec = Rectangle { static_cast<float>(screenWidth / 2 - 50), static_cast<float>(screenHeight / 2 - 20), 100, 40 };

    bool start = false;
    while (!start) {
        BeginDrawing();

        if (GuiButton(rec, "START")) start = true;

        EndDrawing();
    }


    playLevel(registry, *scene, 1);




    CloseWindow();

    return 0;
}
