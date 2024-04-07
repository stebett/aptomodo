//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <valarray>
#include <raylib.h>
#include "npc.h"
#include "constants.h"
#include <raymath.h>
#include "astar.h"


bool checkCollision(const entt::registry &registry, const int id, Vector2 position, Radius radius,
                    const Map &grid) {
    auto enemyView = registry.view<Living, Radius, Position, ID>();
    for (auto [enemy, enemyRadius, enemyPosition, enemyID]: enemyView.each()) {
        if (enemyID.value == id) continue;
        if (CheckCollisionCircles(position, radius.value, {enemyPosition.x, enemyPosition.y}, enemyRadius.value)) {
            return true;
        }
    }
    if (position.x > 0 && position.y > 0 && position.x < mapWidth && position.y < mapHeight &&
        grid((int) position.x / tileSize, (int) position.y / tileSize) != -1)
        if (CheckCollisionCircleRec(position, radius.value, {position.x, position.y, tileSize, tileSize})) {
            return true;
        }
    return false;
}

bool playerInView(const Vector2 position, const Vector2 &playerPosition, const float range) {
    return CheckCollisionCircles(position, range, playerPosition, 1.0f);
}

void updateEnemy(entt::registry &registry, Position &playerPosition, const Map &grid) {
    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID>();
    Position newPosition = {0, 0};
    for (auto [enemy, speed, radius, health, position, id]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            return;
        }

        if (!playerInView(position, playerPosition, 150.0f)) {
//            DrawCircleV(position, 150.0f, ColorAlpha(WHITE, 0.1));
            continue;
        }

        Node start = getTile(position);
        Node end = getTile(playerPosition);
        Search search(grid);
        search.init(start, end);

        // Check distance


        if (!CheckCollisionCircles(playerPosition, radius.value, position, static_cast<float>(2 * tileSize))) {
            while (!search.completed) { search.step(); }
//            search.draw();
            if (!search.path.empty()) {
                newPosition = Vector2Lerp(position, {static_cast<float>(search.path[2].x * tileSize),
                                                     static_cast<float>(search.path[2].y * tileSize)}, 0.05);
            }
        }
        if (!checkCollision(registry, id.value, newPosition, radius, grid)) {
            position = newPosition;
        } else {
//            position = Vector2Subtract(position, {static_cast<float>(search.path[2].x * tileSize),
//                                                  static_cast<float>(search.path[2].y * tileSize)});
        }
    }

}
