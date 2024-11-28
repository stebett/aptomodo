//
// Created by ginko on 11/26/24.
//

#include "game.h"

#include <camera.h>
#include <components.h>
#include <factories.h>
#include <player_ui.h>
#include <ai/loop.h>
#include <systems/physics.h>
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

// TODO find a better place for this
void PlayerFaceMouse(entt::registry& registry, const entt::entity player, const Camera2D &camera) {
    auto &lookAngle = registry.get<LookAngle>(player);
    const auto position = registry.get<Position>(player);
    const auto [mouseX, mouseY] = GetScreenToWorld2D(GetMousePosition(), camera);
    lookAngle = atan2(mouseY - position.y, mouseX - position.x) * RAD2DEG;
}


/*
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayLevel(const int levelNumber) {
    auto outcome = LevelOutcome::NONE;
    entt::registry registry;
    Gui::Instantiate();
    Physics::Instantiate();

    GameCamera camera {};

    spawnEntities(registry, Level::LoadEntities(Assets::GetLevel(levelNumber)));
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(levelNumber));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(levelNumber));
    Assets::Clean();

    const auto player = registry.view<Player>().front();
    const auto &playerPosition = registry.get<Position>(player);
    const auto &health = registry.get<Health>(player);

    FramerateManager framerateManager;

    while (!Game::IsLevelFinished()) {
        if (!Game::IsPaused()) {
            framerateManager.accumulator += framerateManager.deltaTime;
            while (framerateManager.accumulator >= Physics::timeStep) {
                Physics::Step(); // TODO properly update at fixed timestep
                framerateManager.accumulator -= Physics::timeStep;
            }
            Physics::Update(registry);

            Space::Update(registry, camera.GetPlayerCamera());
            AI::Update(registry, player);
            PlayerFaceMouse(registry, player, camera);
        }
        Gui::Update(registry, camera);
        camera.Update(playerPosition, framerateManager.deltaTime);
        Audio::Update(registry);

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(WHITE);
        Rendering::DrawLevel(camera.GetPlayerCamera());

        Inputs::Listen(registry, camera, framerateManager.deltaTime);
        Inputs::Update(registry);
        Rendering::Draw(registry, camera.GetPlayerCamera(), framerateManager.framesCounter); // This has to stay after updatePlayer
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
