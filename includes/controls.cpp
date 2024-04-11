//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "controls.h"
#include "collisions.h"
#include "constants.h"


void playerAttack(entt::registry &registry, entt::entity player, Vector2 clickPosition) {
    Position playerPosition = registry.get<Position>(player);
    float attackRange  = registry.get<AttackRange>(player).value;
    float attackSpread  = registry.get<Spread>(player).value;
    float damage  = registry.get<Damage>(player).value;
    float pushback = registry.get<Pushback>(player).value;

    float click_angle = atan2(clickPosition.y - playerPosition.y, clickPosition.x - playerPosition.x) * radToDeg;
    DrawCircleSector(playerPosition, attackRange, click_angle - attackSpread,
                     click_angle + attackSpread, 2, RED);
    Vector2 endSegment1 = {
            playerPosition.x + attackRange* (float) cos((click_angle - attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((click_angle - attackSpread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + attackRange * (float) cos((click_angle + attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((click_angle + attackSpread) * degToRad)};

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircleTriangle(position, radius.value, playerPosition,
                                         endSegment1, endSegment2, attackRange)) {
            health.value -= attackRange;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * pushback / m),
                        position.y + (position.y - playerPosition.y) * pushback / m};
        }
    }
}

void playerSecondaryAttack(entt::registry &registry, entt::entity player) {
    Position playerPosition = registry.get<Position>(player);
    float attackRange  = registry.get<AttackRange>(player).value;
    float damage  = registry.get<Damage>(player).value;
    float pushback = registry.get<Pushback>(player).value;
    DrawCircle(playerPosition.x, playerPosition.y, attackRange, RED);

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircles(position, radius.value, playerPosition,
                                  attackRange)) {
            health.value -= damage * 2;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * pushback / m * 3),
                        position.y + (position.y - playerPosition.y) * pushback / m * 3};
        }
    }
}

void castFire(entt::registry &registry, entt::entity player, Vector2 clickPosition) {}

void parseInput(entt::registry &registry, entt::entity &player, Position &position, Camera2D &camera, const Map &grid) {
    Radius radius = registry.get<Radius>(player);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        playerAttack(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        castFire(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        playerSecondaryAttack(registry, player);
    }

    static Position futurePos = position;

//    if (playerState != State::pain) {
    futurePos.y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
    futurePos.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    futurePos.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    futurePos.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));
//    }
    if (futurePos.x - radius.value < 0 || futurePos.x + radius.value > mapWidth) {
        futurePos.x = position.x;
    }
    if (futurePos.y - radius.value < 0 || futurePos.y + radius.value > mapHeight) {
        futurePos.y = position.y;
    }
    solveCircleRecCollision(futurePos, radius, grid);
    position = futurePos;

}


