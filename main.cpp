#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "includes/components.h"
#include "includes/constants.h"
#include "includes/controls.h"
#include "includes/levels.h"
#include "includes/rendering.h"
#include "includes/gui.h"
#include "includes/factories.h"
#include "includes/levels.h"
#include "includes/npc.h"


int main() {
    entt::registry registry;
    InitWindow(screenWidth, screenHeight, "Apto Modo");
    GameScene* scene = new GameScene(registry);

    entt::entity player = spawnPlayer(registry, scene, Level::One);

    Position &position = registry.get<Position>(player);
    Pain &pain = registry.get<Pain>(player);
    Health &health = registry.get<Health>(player);

    GUI gui = GUI(screenWidth, screenHeight, pain.value, pain.max, health.value, health.max, "gui_style.rgs");


    SetTargetFPS(60);


    int enemyCounter = 0;

    Camera2D camera = spawnCamera();
    unsigned int framesCounter = 0;
    auto level = Level::One;

    while (!WindowShouldClose() ) {
        updateCamera(camera, position);


        if (framesCounter % 60 == 0 && enemyCounter < 15) {
            Position randomPos = {static_cast<float>(rng::uniform(rng::seed)),
                                  static_cast<float> (rng::uniform(rng::seed))};
            spawnEnemy(registry, randomPos, level);
            ++enemyCounter;
        }


        BeginDrawing();

        ClearBackground(level.color);

        BeginMode2D(camera);
        draw(registry, scene, framesCounter);
        parseInput(registry, player, position, camera);
        updateEnemy(registry, position);

        EndMode2D();
        gui.drawGameplay();


        EndDrawing();

        ++framesCounter;
    }



    CloseWindow();

    return 0;
}
