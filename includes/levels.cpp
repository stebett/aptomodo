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
#include "raygui.h"

Weapon swordByLevel(int level) {
    switch (level) {
        case 1:
            return {10, 100, 25, 150};
        case 2:
            return {10, 100, 25, 150};
        case 3:
            return {10, 100, 25, 150};
        case 4:
            return {10, 100, 25, 150};
        case 5:
            return {10, 100, 25, 150};
        default:
            return {10, 100, 90, 2000};
    }
}

float radiusByLevel(int level) {
    switch (level) {
        case 1:
            return 15;
        case 2:
            return 25;
        case 3:
            return 35;
        case 4:
            return 45;
        case 5:
            return 55;
        default:
            return 1;
    }
}

float painByLevel(int level) {
    switch (level) {
        case 1:
            return 200;
        case 2:
            return 300;
        case 3:
            return 400;
        case 4:
            return 500;
        case 5:
            return 600;
        default:
            return 999;
    }
}

Color colorByLevel(int level) {
    switch (level) {
        case 1:
            return BLACK;
        case 2:
            return BLUE;
        case 3:
            return GREEN;
        case 4:
            return GRAY;
        case 5:
            return RED;
        default:
            return WHITE;
    }
}

void updateCamera(Camera2D &camera, Position &playerPosition) {
    float target_x = playerPosition.x;
    float target_y = playerPosition.y;

    if (playerPosition.x < mapWidth / (2 * camera.zoom)) { target_x = mapWidth / (2 * camera.zoom); }
    if (playerPosition.x > mapWidth - mapWidth / (2 * camera.zoom)) {
        target_x = mapWidth - mapWidth / (2 * camera.zoom);
    }
    if (playerPosition.y < mapHeight / (2 * camera.zoom)) { target_y = mapHeight / (2 * camera.zoom); }
    if (playerPosition.y > mapHeight - mapHeight / (2 * camera.zoom)) {
        target_y = mapHeight - mapHeight / (2 * camera.zoom);
    }

    camera.target = {target_x, target_y};
}


void playLevel(entt::registry &registry, GameScene &scene, int level) {

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
    float painAtStart = pain.value;

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

        DrawRectangleV(topLeft, Vector2{screenWidth, 35}, {250, 200, 200, 150});
//        DrawFPS((int) topLeft.x + 5, (int) topLeft.y + 5);

        GuiProgressBar({topLeft.x + 5, topLeft.y + 5, 60, 20}, std::to_string((int) pain.value).c_str(), "Pain",
                       &pain.value, painAtStart, painByLevel(level));
        GuiProgressBar({topLeft.x + 200, topLeft.y + 5, 30, 20}, std::to_string((int) health.value).c_str(), "Health",
                       &health.value, 0, health.max);
        GuiStatusBar({screenWidth - 100, topLeft.y + 5, 30, 20}, std::to_string(GetFPS()).c_str());

        EndDrawing();

        ++framesCounter;
    }
    registry.destroy(player);
}
