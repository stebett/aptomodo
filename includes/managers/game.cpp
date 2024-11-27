//
// Created by ginko on 11/26/24.
//

#include "game.h"

#include <components.h>
#include <factories.h>
#include <player_ui.h>
#include <ai/loop.h>
#include <systems/space.h>

#include "assets.h"
#include "audioManager.h"
#include "commands.h"
#include "framerateManager.h"
#include "gui.h"
#include "renderingManager.h"

Texture2D Game::levelTexture;
IntGrid Game::grid;
bool Game::paused = false;
bool Game::levelFinished = false;
int Game::Level = 0;
LevelOutcome Game::levelOutcome = LevelOutcome::NONE;

/* TODO
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayLevel(const int levelNumber) {
    auto outcome = LevelOutcome::NONE;
    entt::registry registry;
    Gui::Instantiate();

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

    while (!Game::IsLevelFinished()) {
        if (!Game::IsPaused()) Space::Update(registry, playerCamera);
        Gui::Update(registry, playerCamera);
        Rendering::UpdateCamera(playerCamera, position, framerateManager.deltaTime);
        Audio::Update(registry);
        if (!Game::IsPaused()) AI::Update(registry, player);

        BeginDrawing();
        if (!Config::GetBool("free_camera")) {
            // TODO envelop this in a function
            BeginMode2D(playerCamera);
            freeCamera = playerCamera;
        } else
            BeginMode2D(freeCamera);

        ClearBackground(WHITE);
        Rendering::DrawLevel(playerCamera);

        Inputs::Listen(registry, framerateManager.deltaTime);
        Inputs::Update(registry);
        Rendering::Draw(registry, playerCamera, framerateManager.framesCounter); // This has to stay after updatePlayer
        EndMode2D();
        PlayerUI::Draw(health.value);
        Gui::Draw();

        EndDrawing();
        framerateManager.Update();
    }

    return Game::GetOutcome();
}

void Game::Loop() {
    while (levelOutcome != LevelOutcome::QUIT)
        PlayLevel(Level);
}

bool Game::IsPaused() {
    return paused;
};

void Game::Pause() {
    paused = !paused;
}

bool Game::IsLevelFinished() {
    return levelFinished;
}

void Game::ExitLevel() {
    levelFinished = true;
}

LevelOutcome Game::GetOutcome() {
    return levelOutcome;
}

void Game::SetOutcome(const LevelOutcome outcome) {
    levelOutcome = outcome;
}
