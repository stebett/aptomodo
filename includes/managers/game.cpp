//
// Created by ginko on 11/26/24.
//

#include "game.h"

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

entt::registry Game::registry;
GameCamera Game::camera;
FramerateManager Game::framerateManager{};

// TODO find a better place for this
void PlayerFaceMouse(const entt::entity player, const Camera2D &camera) {
    auto &lookAngle = Game::registry.get<LookAngle>(player);
    const auto body = Game::registry.get<b2BodyId>(player);
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
    Gui::Instantiate();
    Physics::Instantiate();

    GameCamera camera{};

    spawnEntities(Level::LoadEntities(Assets::GetLevel(levelNumber)));
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(levelNumber));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(levelNumber));
    // Assets::Clean();

    const auto player = Game::registry.view<Player>().front();
    const auto &playerPosition = Game::registry.get<Position>(player);
    const auto &health = Game::registry.get<Health>(player);

    FramerateManager framerateManager;

    while (!Game::IsLevelFinished()) {
        if (!Game::IsPaused()) {
            StatusEffect::Update();
            Attacks::Update();
            framerateManager.accumulator += framerateManager.deltaTime;
            while (framerateManager.accumulator >= Physics::timeStep) {
                Physics::Step();
                framerateManager.accumulator -= Physics::timeStep;
            }
            Physics::Update();

            Space::Update(camera.GetPlayerCamera());
            AI::Update(player);
            PlayerFaceMouse(player, camera);
        }
        Gui::Update(camera);
        camera.Update(playerPosition, framerateManager.deltaTime);
        Audio::Update();

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(WHITE);
        Rendering::DrawLevel(camera.GetPlayerCamera());

        const auto commands = Inputs::Listen(camera, framerateManager.deltaTime);
        Inputs::Update(commands);
        Rendering::Draw(camera.GetPlayerCamera(), framerateManager.framesCounter);
        // This has to stay after updatePlayer
        EndMode2D();

        PlayerUI::Draw(health.value);
        Gui::Draw();

        EndDrawing();

        framerateManager.Update();
    }
    Physics::Clean();
    Gui::Clean();
    return Game::GetOutcome();
}

void Game::Loop() {
    SPDLOG_INFO("Entering Loop");

    while (Game::levelOutcome != LevelOutcome::QUIT)
        PlayLevel(Level);
//        PlayAnimationEditorLevel();
}

/*
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome Game::PlayLevelOnce() {
    SPDLOG_INFO("Initializing player");

    const auto player = Game::registry.view<Player>().front();
    const auto &playerPosition = Game::registry.get<Position>(player);
    const auto &health = Game::registry.get<Health>(player);

    if (!Game::IsPaused()) {
        SPDLOG_INFO("Updates");
        StatusEffect::Update();
        Attacks::Update();
        framerateManager.accumulator += framerateManager.deltaTime;
        while (framerateManager.accumulator >= Physics::timeStep) {
            Physics::Step();
            framerateManager.accumulator -= Physics::timeStep;
        }
        Physics::Update();

        Space::Update(camera.GetPlayerCamera());
        AI::Update(player);
        PlayerFaceMouse(player, camera);
    }
    Gui::Update(camera);
    camera.Update(playerPosition, framerateManager.deltaTime);
    Audio::Update();

    SPDLOG_INFO("End Updates");
    BeginDrawing();
    BeginMode2D(camera);
    SPDLOG_INFO("Begin Drawing");


    ClearBackground(WHITE);
    Rendering::DrawLevel(camera.GetPlayerCamera());

    auto commands = Inputs::Listen(camera, framerateManager.deltaTime);
    Inputs::Update(commands);
    Rendering::Draw(camera.GetPlayerCamera(), framerateManager.framesCounter);
    // This has to stay after updatePlayer
    EndMode2D();

    PlayerUI::Draw(health.value);

    Gui::Draw();
    SPDLOG_INFO("End Drawing");

    EndDrawing();

    framerateManager.Update();
    SPDLOG_INFO("Post-updates done");

    return Game::levelOutcome;
}


void Game::PrepareLevel(const int levelNumber) {
    Game::EnterLevel();

    // Initialize registry
    registry = entt::registry{};

    // Instantiate systems
    Gui::Instantiate();
    Physics::Instantiate();

    // Initialize camera
    camera = GameCamera{};

    // Spawn entities
    spawnEntities(Level::LoadEntities(Assets::GetLevel(levelNumber)));

    // Load level data
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(levelNumber));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(levelNumber));

    // Initialize framerate manager
    framerateManager = FramerateManager{};
}

void Game::CleanLevel() {
    Physics::Clean();
    Gui::Clean();

    // Reset static members
    registry = entt::registry{};
    camera = GameCamera{};
    framerateManager = FramerateManager{};
}

void Game::LoopOnce() {
    SPDLOG_INFO("Entering Emscripten Loop");
    PlayLevelOnce();
//
//    PlayAnimationEditorLevelOnce();
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
    registry = entt::registry();
    levelFinished = false;
    levelOutcome = LevelOutcome::NONE;
}

LevelOutcome Game::GetOutcome() {
    return levelOutcome;
}

void Game::SetOutcome(const LevelOutcome outcome) {
    levelOutcome = outcome;
}
