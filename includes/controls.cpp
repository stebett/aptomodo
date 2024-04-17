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
#include "rendering.h"


void playerAttack(entt::registry &registry, entt::entity player, Vector2 clickPosition) {
    auto &attackTimer  = registry.get<AttackTimer>(player).timer;
    if (attackTimer.Elapsed() < registry.get<AttackSpeed>(player)) return;

    attackTimer.Reset();
    Position &playerPosition = registry.get<Position>(player);
    float attackRange  = registry.get<AttackRange>(player);
    float attackSpread  = registry.get<Spread>(player);
    float damage  = registry.get<Damage>(player);
    float pushback = registry.get<Pushback>(player);

    float clickAngle = atan2(clickPosition.y - playerPosition.y, clickPosition.x - playerPosition.x) * radToDeg;

    registry.emplace<AttackEffect>(registry.create(), 100, playerPosition, attackRange, clickAngle - attackSpread, clickAngle + attackSpread, PURPLE);


    Vector2 endSegment1 = {
            playerPosition.x + attackRange* (float) cos((clickAngle - attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((clickAngle - attackSpread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + attackRange * (float) cos((clickAngle + attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((clickAngle + attackSpread) * degToRad)};

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircleTriangle(position, radius, playerPosition,
                                         endSegment1, endSegment2, attackRange)) {
            health -= damage;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * pushback / m),
                        position.y + (position.y - playerPosition.y) * pushback / m};
        }
    }
}

void playerSecondaryAttack(entt::registry &registry, entt::entity player) {
    Position playerPosition = registry.get<Position>(player);
    float attackRange  = registry.get<AttackRange>(player);
    float damage  = registry.get<Damage>(player);
    float pushback = registry.get<Pushback>(player);
    DrawCircle(playerPosition.x, playerPosition.y, attackRange, RED);

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircles(position, radius, playerPosition,
                                  attackRange)) {
            health -= damage * 2;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * pushback / m * 3),
                        position.y + (position.y - playerPosition.y) * pushback / m * 3};
        }
    }
}

void castFire(entt::registry &registry, entt::entity player, Vector2 clickPosition) {}

void parseInput(entt::registry &registry, entt::entity &player, Position &position, Camera2D &camera, const Map &grid) {
    Radius radius = registry.get<Radius>(player);
    Speed speed = registry.get<Speed>(player);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        playerAttack(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        castFire(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        playerSecondaryAttack(registry, player);
    }

    camera.zoom += GetMouseWheelMove()/10;

    static Vector2 futurePos = position;

//    if (playerState != State::pain) {
    futurePos.y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
    futurePos.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    futurePos.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    futurePos.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));
//    }
    if (futurePos.x - radius < 0 || futurePos.x + radius > mapWidth) {
        futurePos.x = position.x;
    }
    if (futurePos.y - radius < 0 || futurePos.y + radius > mapHeight) {
        futurePos.y = position.y;
    }
    Vector2 direction = Vector2Subtract(futurePos, position);
    Vector2 movement = Vector2Scale(Vector2Normalize(direction), speed);
    futurePos = Vector2Add(position, movement);
    solveCircleRecCollision(futurePos, radius, grid);
    position = futurePos;

}


