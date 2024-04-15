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


void solveCollisionEnemy(const entt::registry &registry, const int id, Position &futurePos, Radius radius, const Map &grid) {
    static Vector2 distance;
    static Vector2 enemyPos;
    static float overlap;
    auto enemyView = registry.view<Living, Radius, Position, ID>();
    for (auto [enemy, enemyRadius, enemyPosition, enemyID]: enemyView.each()) {
        if (enemyID.value == id) continue;
        enemyPos = {enemyPosition.x, enemyPosition.y};
        if (CheckCollisionCircles(futurePos, radius.value, enemyPos, enemyRadius.value)) {
            distance = Vector2Subtract(enemyPos, futurePos);
            overlap = radius.value + enemyRadius.value - Vector2Length(distance);
            if (overlap > 0) {
                futurePos = Vector2Subtract(futurePos, Vector2Scale(Vector2Normalize(distance), overlap));
                solveCircleRecCollision(futurePos, radius, grid);
            }
        }
    }
}


bool playerInView(const Vector2 position, const Vector2 &playerPosition, const float range) {
    return CheckCollisionCircles(position, range, playerPosition, 1.0f);
}


void enemyAttack(entt::registry &registry, const entt::entity enemy, entt::entity player, Position position) {
    auto &playerPosition = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);
    auto radius = registry.get<Radius>(player);
    float attackRange = registry.get<AttackRange>(enemy).value;
    float attackSpread = registry.get<Spread>(enemy).value;
    float damage = registry.get<Damage>(enemy).value;
//    float pushback = registry.get<Pushback>(enemy).value;

    float click_angle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;

    DrawCircleSector(position, attackRange, click_angle - attackSpread,
                     click_angle + attackSpread, 2, RED);
    Vector2 endSegment1 = {
            playerPosition.x + attackRange * (float) cos((click_angle - attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((click_angle - attackSpread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + attackRange * (float) cos((click_angle + attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((click_angle + attackSpread) * degToRad)};

    if (CheckCollisionCircleTriangle(playerPosition, radius.value, position,
                                     endSegment1, endSegment2, attackRange)) {
        health.value -= damage;
//        float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
//        playerPosition = {(playerPosition.x + (playerPosition.x - position.x) * pushback / m),
//                          playerPosition.y + (playerPosition.y - position.y) * pushback / m};
    }
}


void updateEnemy(entt::registry &registry, entt::entity &player, const Map &grid) {
    Position futurePos = {0, 0};
    Position playerPosition = registry.get<Position>(player);

    Vector2 target;
    Vector2 direction;
    Vector2 movement;
    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID>();
    for (auto [enemy, speed, radius, health, position, id]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            continue;
        }

        if (!playerInView(position, playerPosition, 200.0f)) {
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
        if (search.path.empty()) return;

//        search.draw();

        target = {static_cast<float>(search.path[2].x * tileSize + tileSize / 2),
                  static_cast<float>(search.path[2].y * tileSize + tileSize / 2)};

        direction = Vector2Subtract(target, position);
        movement = Vector2Scale(Vector2Normalize(direction), speed.value);
        futurePos = Vector2Add(position, movement);


        solveCollisionEnemy(registry, id.value, futurePos, radius, grid);
        solveCircleRecCollision(futurePos, radius, grid);


        position = futurePos;
    }

}
