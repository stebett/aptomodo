//
// Created by ginko on 02/03/24.
//

#include "levels.h"
#include "factories.h"
#include "controls.h"
#include "npc.h"
#include "constants.h"
#include <raylib.h>
#include <random>
#include <string>

Weapon swordByLevel(Level level) {
    switch (level) {
        case Level::One:
            return {10, 100, 25, 150};
        case Level::Two:
            return {10, 100, 25, 150};
        case Level::Three:
            return {10, 100, 25, 150};
        case Level::Four:
            return {10, 100, 25, 150};
        case Level::Five:
            return {10, 100, 25, 150};
    }
}

float radiusByLevel(Level level) {
    switch (level) {
        case Level::One:
            return 15;
        case Level::Two:
            return 25;
        case Level::Three:
            return 35;
        case Level::Four:
            return 45;
        case Level::Five:
            return 55;
    }
}

float painByLevel(Level level) {
    switch (level) {
        case Level::One:
            return 200;
        case Level::Two:
            return 300;
        case Level::Three:
            return 400;
        case Level::Four:
            return 500;
        case Level::Five:
            return 600;
    }
}

Color colorByLevel(Level level) {
    switch (level) {
        case Level::One:
            return BLACK;
        case Level::Two:
            return BLUE;
        case Level::Three:
            return GREEN;
        case Level::Four:
            return GRAY;
        case Level::Five:
            return RED;
    }
}

void updateCamera(Camera2D &camera, Position &playerPosition) {
    float target_x = playerPosition.x;
    float target_y = playerPosition.y;

    if (playerPosition.x < screenWidth / ( 2 * camera.zoom)) {target_x = screenWidth / ( 2 * camera.zoom);}
    if (playerPosition.x > screenWidth - screenWidth / ( 2 * camera.zoom)) {target_x = screenWidth - screenWidth / ( 2 * camera.zoom);}
    if (playerPosition.y < screenHeight / ( 2 * camera.zoom)) {target_y = screenHeight / ( 2 * camera.zoom);}
    if (playerPosition.y > screenHeight - screenHeight / ( 2 * camera.zoom)) {target_y = screenHeight - screenHeight / ( 2 * camera.zoom);}

    camera.target = {target_x, target_y};
}


void playLevel(entt::registry &registry, Level level, GameScene &scene) {

    std::random_device r;
    std::default_random_engine randomEngine(r());
    std::uniform_int_distribution<int> uniform_dist(1, 500);

    entt::entity player = spawnPlayer(registry, level);
    Position &position = registry.get<Position>(player);
    Pain &pain = registry.get<Pain>(player);
    Health &health = registry.get<Health>(player);

    int enemyCounter = 0;

    Camera2D camera = spawnCamera();
    Vector2 topLeft{};
    unsigned int framesCounter = 0;


    while (!WindowShouldClose() && pain.value < painByLevel(level)) {
        updateCamera(camera, position);
        topLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);


        if (framesCounter % 60 == 0 && enemyCounter < 15) {
            Position randomPos = {static_cast<float>(uniform_dist(randomEngine)),
                                  static_cast<float> (uniform_dist(randomEngine))};
            spawnEnemy(registry, randomPos, level);
            ++enemyCounter;
        }

        BeginDrawing();

        ClearBackground(colorByLevel(level));
        BeginMode2D(camera);

        draw(registry, scene);
        parseInput(registry, player, position, camera);
        updateEnemy(registry, position);

        DrawRectangleV(topLeft, Vector2{80, 20}, {0, 0, 0, 100});
        DrawFPS((int) topLeft.x, (int) topLeft.y);

        DrawText(("Pain: " + std::to_string((int) pain.value)).c_str(), topLeft.x, topLeft.y + 30, 25, WHITE);
        DrawText(("Health: " + std::to_string((int) health.value)).c_str(), topLeft.x, topLeft.y + 60, 25, WHITE);
        EndDrawing();

        ++framesCounter;
    }
    registry.destroy(player);
}
