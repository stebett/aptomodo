#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/levels.h"
#include "includes/rendering.h"
#include "includes/gui.h"
#include "includes/factories.h"
#include "includes/levels.h"
#include "includes/npc.h"


int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");
    auto camera = spawnCamera();
    auto scene = new GameScene(registry);
    auto level = Level::One;

    entt::entity player = spawnPlayer(registry, scene, level);
    auto &position = registry.get<Position>(player);
    auto &pain = registry.get<Pain>(player);
    auto &health = registry.get<Health>(player);

    GUI gui = GUI(screenWidth, screenHeight, pain.value, pain.max, health.value, health.max, "gui_style.rgs");


    auto enemyCounter = 0;
    auto framesCounter = 0u;

    SetTargetFPS(60);
    while (!WindowShouldClose() ) {
        updateCamera(camera, position);

        if (framesCounter % 60 == 0 && enemyCounter < 1) {
            spawnRandomEnemy(registry, level);
            ++enemyCounter;
        }

        BeginDrawing();

        BeginMode2D(camera);
        draw(registry, scene, framesCounter);
        parseInput(registry, player, position, camera, scene -> grid);
        updateEnemy(registry, position, scene -> grid);

        EndMode2D();
        gui.drawGameplay();


        EndDrawing();

        ++framesCounter;
    }



    CloseWindow();

    return 0;
}
