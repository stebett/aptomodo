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
#include "collisions.h"


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


void enemyAttack(entt::registry &registry, const entt::entity enemy, entt::entity player, Position position) {
    auto &playerPosition = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);
    auto weapon = registry.get<Weapon>(enemy);
    auto radius = registry.get<Radius>(player);

    float click_angle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;

    DrawCircleSector(position, weapon.range, click_angle - weapon.spread,
                     click_angle + weapon.spread, 2, RED);
    Vector2 endSegment1 = {
            playerPosition.x + weapon.range * (float) cos((click_angle - weapon.spread) * degToRad),
            playerPosition.y + weapon.range * (float) sin((click_angle - weapon.spread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + weapon.range * (float) cos((click_angle + weapon.spread) * degToRad),
            playerPosition.y + weapon.range * (float) sin((click_angle + weapon.spread) * degToRad)};

    if (CheckCollisionCircleTriangle(playerPosition, radius.value, position,
                                     endSegment1, endSegment2, weapon.range)) {
        health.value -= weapon.damage;
        float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
        playerPosition = {(playerPosition.x + (playerPosition.x - position.x) * weapon.pushback / m),
                          playerPosition.y + (playerPosition.y - position.y) * weapon.pushback / m};
    }
}


void updateEnemy(entt::registry &registry, entt::entity &player, const Map &grid) {
    Position newPosition = {0, 0};
    Position playerPosition = registry.get<Position>(player);

    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID>();
    for (auto [enemy, speed, radius, health, position, id]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            return;
        }

        if (!playerInView(position, playerPosition, 300.0f)) {
//            DrawCircleV(position, 150.0f, ColorAlpha(WHITE, 0.1));
            continue;
        }
        if (CheckCollisionCircles(playerPosition, radius.value, position, static_cast<float>(2 * tileSize))) {
            enemyAttack(registry, enemy, player, position);
            continue;
        }
        Node start = getTile(position);
        Node end = getTile(playerPosition);
        Search search(grid);
        search.init(start, end);

        // Check distance


        while (!search.completed) { search.step(); }
//            search.draw();
        if (!search.path.empty()) {
            newPosition = Vector2Lerp(position, {static_cast<float>(search.path[2].x * tileSize),
                                                 static_cast<float>(search.path[2].y * tileSize)}, 0.05);
        }

        if (!checkCollision(registry, id.value, newPosition, radius, grid)) {
            position = newPosition;
        } else {
            newPosition = Vector2Subtract(
                    {static_cast<float>(search.path[2].x * tileSize), static_cast<float>(search.path[2].y * tileSize)},
                    position);
            newPosition = Vector2Rotate(newPosition, rng::uniform_neg500_500(rng::seed)/500*90);
            newPosition = Vector2Scale(newPosition, 0.05);
            position = Vector2Add(position, newPosition);
        }
    }

}
