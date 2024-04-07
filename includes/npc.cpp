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
    if (position.x > mapWidth || position.y > mapHeight ||
        grid((int) position.x / tileSize, (int) position.y / tileSize) == -1) {
        return true;
    }
    return false;
}

void updateEnemy(entt::registry &registry, Position &playerPosition, const Map &grid) {
    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID>();
    for (auto [enemy, speed, radius, health, position, id]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            return;
        }
        Node start = getTile(position);
        Node end = getTile(playerPosition);
        Search search(grid);
        search.init(start, end);
        if (!CheckCollisionCircles(playerPosition, radius.value, position, static_cast<float>(2 * tileSize))) {
            while (!search.completed) { search.step(); }
            search.draw();
            position = Vector2Lerp(position, {static_cast<float>(search.path[2].x * tileSize),
                                              static_cast<float>(search.path[2].y * tileSize)}, 0.05);
        }
////        Position newPosition = Vector2Lerp(Vector2MoveTowards(position, playerPosition, 10.0f), playerPosition, 0.1f);
//        if (!checkCollision(registry, id.value, newPosition, radius, grid)) {
//            position = newPosition;
//        }
    }

}
