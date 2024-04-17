#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/rendering.h"
#include "includes/gui.h"
#include "includes/factories.h"
#include "includes/npc.h"
#include "includes/config.h"


int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");
    auto camera = spawnCamera();
    auto scene = new GameScene(registry);

    auto player = registry.view<Player>().front();
    auto &position = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);

    auto io = InitGui();

    auto enemyCounter = 0;
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
        draw(registry, scene, framesCounter);
        updateEnemy(registry, player, scene->grid);

        EndMode2D();
        parseInput(registry, player, position, camera, scene->grid);

        DrawGui();
        DrawFPS(10, 10);

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
