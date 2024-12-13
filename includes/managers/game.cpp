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
#include "systems/attacks.h"
#include "audioManager.h"
#include "gui.h"
#include "renderingManager.h"
#include "systems/statusUpdate.h"
#include "gui/aiEditor.h"
#include "gui/animationEditor.h"

Texture2D Game::levelTexture;
IntGrid Game::grid;
bool Game::paused = false;
bool Game::levelFinished = false;
int Game::Level {0};
std::function<void()> Game::LevelFunction {&PlayLevel};
LevelOutcome Game::levelOutcome = LevelOutcome::NONE;
Shader Game::testShader;
entt::registry Game::registry;
GameCamera Game::camera;
FramerateManager Game::framerateManager{};

// TODO find a better place for this

void PlayerFaceMouse(const entt::entity player, const Camera2D &camera) {

    auto &lookAngle = Game::registry.get<LookAngle>(player);
    const auto body = Game::registry.get<b2BodyId>(player);
    const auto transform{b2Body_GetTransform(body)};
    const auto position{b2Body_GetPosition(body)};

    const auto [mouseX, mouseY] = GetScreenToWorld2D(GetMousePosition(), camera);
    const auto radians = atan2(mouseY - transform.p.y, mouseX - transform.p.x);
    lookAngle = radians * RAD2DEG;
    if (!Config::GetBool("inEditor"))
        b2Body_SetTransform(body, transform.p, b2Rot(cos(radians), sin(radians)));
    else b2Body_SetTransform(body, position, b2Rot(cos(-1.5708), sin(-1.5708)));;
//    b2Body_SetTransform(body, transform.p, b2Rot(cos(radians), sin(radians)));
}

LevelOutcome PlayLevel() {
    Game::EnterLevel();
    Gui::Instantiate();
    Physics::Instantiate();


    spawnEntities(Level::LoadEntities(Assets::GetLevel(Game::Level)));
    Game::levelTexture = Level::LoadLevelTexture(Assets::GetLevel(Game::Level));
    Game::grid = Level::LoadIntGrid(Assets::GetLevel(Game::Level));

    const auto player = Game::registry.view<Player>().front();
    const auto &playerPosition = Game::registry.get<Position>(player);
    const auto &health = Game::registry.get<Health>(player);

    Game::framerateManager = FramerateManager();


    while (!Game::IsLevelFinished()) {
        if (!Game::IsPaused()) {
            StatusEffect::Update();
            Attacks::Update();
            Game::framerateManager.accumulator += Game::framerateManager.deltaTime;
            while (Game::framerateManager.accumulator >= Physics::timeStep) {
                Physics::Step();
                Game::framerateManager.accumulator -= Physics::timeStep;
            }
            Physics::Update();

            Space::Update(Game::camera.GetPlayerCamera());
            AI::Update(player);
            PlayerFaceMouse(player, Game::camera);
        }
        Gui::Update(Game::camera, &imguiWindowMain);
        Game::camera.Update(playerPosition, Game::framerateManager.deltaTime);
        Audio::Update();

        BeginDrawing();
        BeginMode2D(Game::camera);

        ClearBackground(WHITE);
        BeginShaderMode(Game::testShader);
        Game::UpdateShaders();
        Rendering::DrawLevel(Game::camera.GetPlayerCamera());
        EndShaderMode();
        const auto commands = Inputs::Listen(Game::camera, Game::framerateManager.deltaTime);
        Inputs::Update(commands);
        Rendering::Draw(Game::camera.GetPlayerCamera(), Game::framerateManager.framesCounter);
        // This has to stay after updatePlayer
        EndMode2D();

        PlayerUI::Draw(health.value);
        Gui::Draw();

        EndDrawing();

        Game::framerateManager.Update();
    }
    Physics::Clean();
    Gui::Clean();
    return Game::GetOutcome();
}

void Game::Loop() {
    SPDLOG_INFO("Entering Loop");

    while (Game::levelOutcome != LevelOutcome::QUIT)
        LevelFunction();
//        PlayAIEditor();
//        PlayLevel(Level);
//        PlayAnimationEditorLevel();
}


void Game::ChangeLevel(Level::LevelName newLevel) {
    Game::Level = newLevel;
    switch (newLevel) {
        case Level::First : {
            LevelFunction = &PlayLevel;
            *Config::GetBoolPtr("in_editor_level") = false;
            break;
        }
        case Level::Second:{
            LevelFunction = &PlayLevel;
            *Config::GetBoolPtr("in_editor_level") = false;

            break;
        }
        case Level::Third:{
            LevelFunction = &PlayLevel;
            *Config::GetBoolPtr("in_editor_level") = false;

            break;
        }
        case Level::AIEditor:{
            LevelFunction = &PlayAIEditor;
            *Config::GetBoolPtr("in_editor_level") = true;

            break;
        }
        case Level::AnimationEditor:
            LevelFunction = &PlayAnimationEditorLevel;
            *Config::GetBoolPtr("in_editor_level") = true;

            break;
    }
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
    testShader = LoadShader(nullptr, (std::filesystem::path(ROOT_PATH) / "assets/shaders/base.fs").c_str());

}

LevelOutcome Game::GetOutcome() {
    return levelOutcome;
}

void Game::SetOutcome(const LevelOutcome outcome) {
    levelOutcome = outcome;
}

void Game::UpdateShaders() {
    // Get uniform locations
    int iTimeLoc = GetShaderLocation(Game::testShader, "iTime");
    int iResolutionLoc = GetShaderLocation(Game::testShader, "iResolution");
    int iCenterLoc = GetShaderLocation(Game::testShader, "iCenter");
    Vector2 resolution = { (float)GetScreenWidth(), (float)GetScreenHeight() };

    float time = GetTime(); // Get time in seconds
    const auto player = Game::registry.view<Player>().front();
    const auto playerPosition = Game::registry.get<Position>(player);
    Vector2 screenPos = GetWorldToScreen2D(playerPosition, Game::camera);
    Vector2 normalizedPos = {
            (screenPos.x / resolution.x) * 2.0f - 1.0f,
            (screenPos.y / resolution.y) * 2.0f - 1.0f
    };
//    Vector2 center = GetMousePosition();
    SetShaderValue(Game::testShader, iTimeLoc, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(Game::testShader, iResolutionLoc, &resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(Game::testShader, iCenterLoc, &normalizedPos, SHADER_UNIFORM_VEC2);
}