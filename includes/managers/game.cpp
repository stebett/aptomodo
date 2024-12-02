//
// Created by ginko on 11/26/24.
//

#include "game.h"

#include <camera.h>
#include <components.h>
#include <factories.h>
#include <player_ui.h>
#include <rlImGui.h>
#include <ai/loop.h>
#include <systems/inputs.h>
#include <systems/physics.h>
#include <systems/space.h>

#include "assets.h"
#include "attacks.h"
#include "audioManager.h"
#include "commands.h"
#include "framerateManager.h"
#include "gui.h"
#include "renderingManager.h"
#include "systems/statusUpdate.h"
#include "gui/animationEditor.h"

Texture2D Game::levelTexture;
IntGrid Game::grid;
bool Game::paused = false;
bool Game::levelFinished = false;
int Game::Level = 0;
LevelOutcome Game::levelOutcome = LevelOutcome::NONE;

// TODO find a better place for this
void PlayerFaceMouse(entt::registry &registry, const entt::entity player, const Camera2D &camera) {
    auto &lookAngle = registry.get<LookAngle>(player);
    const auto body = registry.get<b2BodyId>(player);
    const auto transform{b2Body_GetTransform(body)};

    const auto [mouseX, mouseY] = GetScreenToWorld2D(GetMousePosition(), camera);
    const auto radians = atan2(mouseY - transform.p.y, mouseX - transform.p.x);
    lookAngle = radians * RAD2DEG;
    if (!Config::GetBool("inEditor"))
        b2Body_SetTransform(body, transform.p, b2Rot(cos(radians), sin(radians)));
}


/*
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayLevel(const int levelNumber) {
    Game::EnterLevel();
    entt::registry registry;
    Gui::Instantiate();
    Physics::Instantiate();

    GameCamera camera{};

    spawnEntities(registry, Level::LoadEntities(Assets::GetLevel(levelNumber)));
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(levelNumber));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(levelNumber));
    // Assets::Clean();

    const auto player = registry.view<Player>().front();
    const auto &playerPosition = registry.get<Position>(player);
    const auto &health = registry.get<Health>(player);

    FramerateManager framerateManager;

    while (!Game::IsLevelFinished()) {
        if (!Game::IsPaused()) {
            StatusEffect::Update(registry);
            Attacks::Update(registry);
            framerateManager.accumulator += framerateManager.deltaTime;
            while (framerateManager.accumulator >= Physics::timeStep) {
                Physics::Step();
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
        Rendering::Draw(registry, camera.GetPlayerCamera(), framerateManager.framesCounter);
        // This has to stay after updatePlayer
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
//        PlayLevel(Level);
        PlayAnimationEditorLevel();
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

void Game::EnterLevel() {
    levelFinished = false;
    levelOutcome = LevelOutcome::NONE;
}

LevelOutcome Game::GetOutcome() {
    return levelOutcome;
}

void Game::SetOutcome(const LevelOutcome outcome) {
    levelOutcome = outcome;
}
