#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/rendering.h"
#include "includes/gui.h"
#include "includes/factories.h"
#include "includes/npc.h"


int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");
    auto camera = spawnCamera();
    auto scene = new GameScene(registry);

    auto player = registry.view<Player>().front();
    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    auto io = InitGui();

    auto enemyCounter = 0;
    auto framesCounter = 0u;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        updateCamera(camera, position);
        UpdateGui(registry, io);

        BeginDrawing();

        BeginMode2D(camera);
        draw(registry, scene, framesCounter);
        updateEnemy(registry, player, scene->grid);

        EndMode2D();
        parseInput(registry, player, position, camera, scene->grid);

        DrawGui();
        DrawFPS(10, 10);

        EndDrawing();

        ++framesCounter;
    }

    CloseGui();
    CloseWindow();

    return 0;
}
