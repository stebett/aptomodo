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

enum class LevelOutcome {
    WIN,
    LOSE,
    RESTART,
    QUIT,
    NONE,
};

LevelOutcome PlayLevel() {
    LevelOutcome outcome = LevelOutcome::NONE;
    entt::registry registry;
    RenderingManager::Instantiate(registry);


    auto camera = spawnCamera();
    auto player = spawnPlayer(registry);
    spawnEnemies(registry);
    spawnItems(registry);

    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    auto io = InitGui();

    FramerateManager framerateManager;

    bool windowsShouldClose = false;
    bool paused = false;
    while (!windowsShouldClose) {
        UpdateGui(registry, io);
        updateCamera(camera, position);
        BeginDrawing();
        BeginMode2D(camera); // TODO add option to activate second debug camera
        ClearBackground(WHITE);
        LevelManager::Draw(camera);
        RenderingManager::Draw(camera, framerateManager.framesCounter);
        if (!paused) {
            updateEnemy(registry, player); // TODO This should be before drawing
            updatePlayer(registry, player, position, camera);
        }
        EndMode2D();
        DrawGui();
        DrawFPS(10, 10);

        DrawText(std::format("Health: {}", health.value).c_str(), 10, screenHeight - 40, 30, WHITE);
        if (paused) {
            DrawText("PAUSE", screenHeight / 2 - 50, screenWidth / 2 - MeasureText("PAUSE", 50), 50, WHITE);
        }

        EndDrawing();

        framerateManager.Update();
        if (IsKeyPressed(KEY_Q)) {
            windowsShouldClose = true;
            outcome = LevelOutcome::QUIT;
        }
        if (IsKeyPressed(KEY_R)) {
            windowsShouldClose = true;
            outcome = LevelOutcome::RESTART;
        }
        if (IsKeyPressed(KEY_P)) { paused = !paused; }
    }

    return
            outcome;
}

void GameLoop() {
    LevelOutcome outcome = LevelOutcome::NONE;
    do {
        outcome = PlayLevel();
    } while (outcome != LevelOutcome::QUIT);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Apto Modo");

    AudioManager::Instantiate();
    AnimationManager::Instantiate();
    LevelManager::Instantiate();

    GameLoop();
    CloseGui();
    CloseWindow();
    return 0;
}
