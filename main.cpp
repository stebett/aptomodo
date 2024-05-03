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

// TODO move rllImGui, ImGui, raygui, and raylib in vendors dir
int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");

    RenderingManager::Instantiate(registry);
    AudioManager::Instantiate();
    AnimationManager::Instantiate();
    LevelManager::Instantiate();

    auto camera = spawnCamera();

    auto player = spawnPlayer(registry);
    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    auto io = InitGui();

    auto framesCounter = 0u;


    // Custom timming variables
    double previousTime = GetTime();    // Previous time measure
    double currentTime = 0.0;           // Current time measure
    double updateDrawTime = 0.0;        // Update + Draw time
    double waitTime = 0.0;              // Wait time (if target fps required)
    float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)

    float timeCounter = 0.0f;           // Accumulative time counter (seconds)

//    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        updateCamera(camera, position);
        UpdateGui(registry, io);

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(WHITE);
        LevelManager::Draw(camera);
        RenderingManager::Draw(framesCounter);

        updateEnemy(registry, player); // TODO This should be before drawing

        EndMode2D();
        parseInput(registry, player, position, camera);

        DrawGui();
        DrawFPS(10, 10);

        DrawText(std::format("Health: {}", health.value).c_str(), 10, screenHeight -40, 30, WHITE);

        EndDrawing();

        ++framesCounter;
        currentTime = GetTime();
        updateDrawTime = currentTime - previousTime;

        if (config::fps > 0)          // We want a fixed frame rate
        {
            waitTime = (1.0f/(float)config::fps) - updateDrawTime;
            if (waitTime > 0.0)
            {
                WaitTime((float)waitTime);
                currentTime = GetTime();
                deltaTime = (float)(currentTime - previousTime);
            }
        }
        else deltaTime = (float)updateDrawTime;    // Framerate could be variable

        previousTime = currentTime;
    }

    CloseGui();
    CloseWindow();
    return 0;
}
