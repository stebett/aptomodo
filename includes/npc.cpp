//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <valarray>
#include <raylib.h>
#include "npc.h"
#include "constants.h"
#include <raymath.h>



bool checkCollision(const entt::registry &registry, Vector2 position, Radius radius, const Map &grid) {
    auto enemyView = registry.view<Living, Radius, Position>();
    for (auto [enemy, enemyRadius, enemyPosition]: enemyView.each())
        if (CheckCollisionCircles(position, radius.value, {enemyPosition.x, enemyPosition.y}, enemyRadius.value))
            return true;
        if (position.x > mapWidth || position.y > mapHeight || grid((int)position.x / tileSize, (int)position.y / tileSize) == -1) {
            return true;
            }
    return false;
}

void updateEnemy(entt::registry &registry, Position &playerPosition, const Map &grid) {
    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy>();
    for (auto [enemy, speed, radius, health, position]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            return;
        }
        Position newPosition = Vector2MoveTowards(position, playerPosition, speed.value);
        if (!checkCollision(registry, newPosition, radius, grid)) {
            position = newPosition;
        }
    }
}
