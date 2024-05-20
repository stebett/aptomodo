// TODO move raylib in vendors dir
#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/gui.h"
#include "includes/factories.h"
#include "includes/npc.h"
#include "includes/config.h"
#include "includes/rendering.h"
#include <format>
#include "includes/managers/audioManager.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/renderingManager.h"
#include "includes/managers/levelManager.h"
#include "managers/framerateManager.h"

int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");

    RenderingManager::Instantiate(registry);
    AudioManager::Instantiate();
    AnimationManager::Instantiate();
    LevelManager::Instantiate();

    auto camera = spawnCamera();
    auto player = spawnPlayer(registry);
    spawnEnemies(registry);
    spawnItems(registry);

    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    auto io = InitGui();

    FramerateManager framerateManager;
//    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        updateCamera(camera, position);
        UpdateGui(registry, io);

        BeginDrawing();
        BeginMode2D(camera); // TODO add option to activate second debug camera

        ClearBackground(WHITE);
        LevelManager::Draw(camera);
        RenderingManager::Draw(camera, framerateManager.framesCounter);

        updateEnemy(registry, player); // TODO This should be before drawing

        EndMode2D();
        updatePlayer(registry, player, position, camera);

        DrawGui();
        DrawFPS(10, 10);

        DrawText(std::format("Health: {}", health.value).c_str(), 10, screenHeight -40, 30, WHITE);

        EndDrawing();
        framerateManager.Update();
    }

    CloseGui();
    CloseWindow();
    return 0;
}
