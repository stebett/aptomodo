#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/levels.h"


int main() {
    entt::registry registry;


    InitWindow(screenWidth, screenHeight, "Apto Modo");
    SetTargetFPS(60);


    playLevel(registry, Level::One);
    playLevel(registry, Level::Two);
    playLevel(registry, Level::Three);
    playLevel(registry, Level::Four);
    playLevel(registry, Level::Five);


    CloseWindow();

    return 0;
}
