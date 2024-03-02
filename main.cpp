#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/levels.h"
#include "includes/rendering.h"


int main() {
    entt::registry registry;

    InitWindow(screenWidth, screenHeight, "Apto Modo");
    GameScene* scene = new GameScene;

    SetTargetFPS(60);


    playLevel(registry, Level::One, *scene);



    CloseWindow();

    return 0;
}
