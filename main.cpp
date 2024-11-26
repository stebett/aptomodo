#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/factories.h"
#include "includes/config.h"
#include <ai/loop.h>

#include "commands.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/renderingManager.h"
#include "includes/managers/levelManager.h"
#include "includes/managers/parameters.h"
#include "includes/managers/gui.h"
#include "managers/assets.h"
#include "managers/framerateManager.h"

/* TODO All the level related stuff and the game loop function should be in another file
 */
enum class LevelOutcome {
    WIN,
    LOSE,
    RESTART,
    QUIT,
    NONE,
};

/* TODO
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayLevel() {
    auto outcome = LevelOutcome::NONE;
    entt::registry registry;
    GuiManager::Instantiate();

    auto playerCamera = spawnCamera();
    auto freeCamera = spawnCamera();

    auto player = spawnPlayer(registry);
    spawnEnemies(registry);
    spawnItems(registry);

    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    FramerateManager framerateManager;

    bool windowsShouldClose = false;
    bool paused = false;
    while (!windowsShouldClose) {
        GuiManager::Update(registry, playerCamera);
        LevelManager::Update(registry);
        RenderingManager::UpdateCamera(playerCamera, position);
        BeginDrawing();
        if (!Config::GetBool("free_camera")) { // TODO envelop this in a function
            BeginMode2D(playerCamera);
            freeCamera = playerCamera;
        }
        else
            BeginMode2D(freeCamera);

        ClearBackground(WHITE);
        RenderingManager::DrawLevel(playerCamera);

        if (!paused) {
            AI::Update(registry, player); // TODO this needs to be before BeginDrawing
            generateCommands(registry);
            commandSystem(registry);
            // updateEnemy(registry, player); // TODO This should be before drawing
            if (Config::GetBool("free_camera")) // TODO this will be removed when commands are finished
                updatePlayer(registry, player, position, freeCamera);
            else {
                // updatePlayer(registry, player, position, playerCamera);
            }
        }
        RenderingManager::Draw(registry, playerCamera, framerateManager.framesCounter); // This has to stay after updatePlayer
        EndMode2D();
        GuiManager::Draw();
        DrawFPS(10, 10);
        DrawText(std::format("Health: {}", health.value).c_str(), 10, screenHeight - 40, 30, WHITE);
        // TODO make a PlayerUI namespace and just PlayerUI::Draw() everything
        if (paused) { // TODO this feels like it should be Game::IsPaused(), put it in a namespace for now
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

    return outcome;
}

void GameLoop() {
    auto outcome = LevelOutcome::NONE;
    do {
        outcome = PlayLevel();
    } while (outcome != LevelOutcome::QUIT);
}

int main() {
    //TODO  put all of these in Game::Instantiate()
    InitWindow(screenWidth, screenHeight, "Apto Modo"); // TODO WindowManager::Instantiate();
    ToggleFullscreen(); // This will be inside WindowManager::Instantiate()
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    LevelManager::Instantiate();
    Params::Instantiate();

    GameLoop(); // Game::Run()
    CloseWindow(); // ~Game
    return 0;
}
