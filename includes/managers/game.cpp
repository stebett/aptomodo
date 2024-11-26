//
// Created by ginko on 11/26/24.
//

#include "game.h"

#include <components.h>
#include <factories.h>
#include <player_ui.h>
#include <ai/loop.h>

#include "assets.h"
#include "audioManager.h"
#include "commands.h"
#include "framerateManager.h"
#include "gui.h"
#include "renderingManager.h"

Texture2D Game::levelTexture;
IntGrid Game::grid;
bool Game::paused = false;

/* TODO
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayLevel(int levelNumber) {
    auto outcome = LevelOutcome::NONE;
    entt::registry registry;
    GuiManager::Instantiate();

    auto playerCamera = spawnCamera();
    auto freeCamera = spawnCamera();

    spawnEntities(registry, Level::LoadEntities(Assets::GetLevel(levelNumber)));
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(levelNumber));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(levelNumber));
    Assets::Clean();

    auto player = registry.view<Player>().front();
    const auto &position = registry.get<Position>(player);
    const auto &health = registry.get<Health>(player);

    FramerateManager framerateManager;

    bool windowsShouldClose = false;
    bool paused = false;
    while (!windowsShouldClose) {
        GuiManager::Update(registry, playerCamera);
        RenderingManager::UpdateCamera(playerCamera, position);
        Audio::Update(registry);
        BeginDrawing();
        if (!Config::GetBool("free_camera")) { // TODO envelop this in a function
            BeginMode2D(playerCamera);
            freeCamera = playerCamera;
        }
        else
            BeginMode2D(freeCamera);

        ClearBackground(WHITE);
        RenderingManager::DrawLevel(playerCamera);

        if (!Game::IsPaused()) {
            AI::Update(registry, player); // TODO this needs to be before BeginDrawing
            generateCommands(registry);
            commandSystem(registry);
        }
        RenderingManager::Draw(registry, playerCamera, framerateManager.framesCounter); // This has to stay after updatePlayer
        EndMode2D();
        PlayerUI::Draw(health.value);
        GuiManager::Draw();


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

void Game::Loop() {
    auto outcome = LevelOutcome::NONE;
    do {
        outcome = PlayLevel(0);
    } while (outcome != LevelOutcome::QUIT);
}

bool Game::IsPaused() {
    return paused;
};

void Game::Pause() {
    paused = true;
}