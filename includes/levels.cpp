//
// Created by ginko on 02/03/24.
//

#include "levels.h"
#include "rendering.h"
#include "factories.h"
#include "controls.h"
#include "npc.h"
#include <raylib.h>
#include <random>



void playLevel(entt::registry &registry, Level level) {

    std::random_device r;
    std::default_random_engine randomEngine(r());
    std::uniform_int_distribution<int> uniform_dist(1, 500);

    entt::entity player = spawnPlayer(registry);
    Position &position = registry.get<Position>(player);
    Pain &pain = registry.get<Pain>(player);

    int enemyCounter = 0;

    Camera2D camera = spawnCamera();
    Vector2 world{};
    unsigned int framesCounter = 0;

    while (!WindowShouldClose() && pain.value < 200.0f) {
        world = GetScreenToWorld2D(Vector2{0, 0}, camera);
        parseInput(registry, player, position, camera);
        updateEnemy(registry, position);

        if (framesCounter % 60 == 0 && enemyCounter < 15) {
            Position randomPos = {static_cast<float>(uniform_dist(randomEngine)),
                                  static_cast<float> (uniform_dist(randomEngine))};
            spawnEnemy(registry, randomPos, level);
            ++enemyCounter;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        BeginMode2D(camera);

        draw(registry);

        DrawRectangleV(world, Vector2{80, 20}, {0, 0, 0, 100});
        DrawFPS((int) world.x, (int) world.y);

        EndDrawing();

        ++framesCounter;
    }
}
