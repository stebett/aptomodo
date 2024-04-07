//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <raylib.h>
#include <iostream>
#include "controls.h"
#include "collisions.h"
#include "constants.h"


void playerAttack(entt::registry &registry, entt::entity player, Vector2 clickPosition) {
    Position playerPosition = registry.get<Position>(player);
    Weapon playerWeapon = registry.get<Weapon>(player);

    float click_angle = atan2(clickPosition.y - playerPosition.y, clickPosition.x - playerPosition.x) * radToDeg;
    DrawCircleSector(playerPosition, playerWeapon.range, click_angle - playerWeapon.spread,
                     click_angle + playerWeapon.spread, 2, RED);
    Vector2 endSegment1 = {
            playerPosition.x + playerWeapon.range * (float) cos((click_angle - playerWeapon.spread) * degToRad),
            playerPosition.y + playerWeapon.range * (float) sin((click_angle - playerWeapon.spread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + playerWeapon.range * (float) cos((click_angle + playerWeapon.spread) * degToRad),
            playerPosition.y + playerWeapon.range * (float) sin((click_angle + playerWeapon.spread) * degToRad)};

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircleTriangle(position, radius.value, playerPosition,
                                         endSegment1, endSegment2, playerWeapon.range)) {
            health.value -= playerWeapon.damage;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * playerWeapon.pushback / m),
                        position.y + (position.y - playerPosition.y) * playerWeapon.pushback / m};
        }
    }
}

void playerSecondaryAttack(entt::registry &registry, entt::entity player) {
    Position playerPosition = registry.get<Position>(player);
    Weapon playerWeapon = registry.get<Weapon>(player);
    DrawCircle(playerPosition.x, playerPosition.y, playerWeapon.range, RED);

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircles(position, radius.value, playerPosition,
                                  playerWeapon.range)) {
            health.value -= playerWeapon.damage * 2;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * playerWeapon.pushback / m * 3),
                        position.y + (position.y - playerPosition.y) * playerWeapon.pushback / m * 3};
        }
    }
}

void castFire(entt::registry &registry, entt::entity player, Vector2 clickPosition) {}

void parseInput(entt::registry &registry, entt::entity &player, Position &position, Camera2D &camera, const Map &grid) {
    State &playerState = registry.get<PlayerState>(player).state;
    Radius radius = registry.get<Radius>(player);
    if (IsKeyPressed(KEY_P) || IsKeyDown(KEY_P)) {
        Pain &pain = registry.get<Pain>(player);
        pain.value += 10;
        playerState = State::pain;
        return;
    }
    if (IsKeyPressed(KEY_ONE)) {
        playerState = State::casting;
    }
    if (IsKeyPressed(KEY_ZERO)) {
        playerState = State::normal;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && playerState == State::normal) {
        playerAttack(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && playerState == State::casting) {
        castFire(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        playerSecondaryAttack(registry, player);
    }

    static int y = position.y;
    static int x = position.x;

//    if (playerState != State::pain) {
    y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
    y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));
//    }

    if (x < 0 || y < 0 || x > mapWidth || y > mapHeight || grid(x / tileSize, y / tileSize) == -1) {
        position.x = x;
        position.y = y;
    } else {
//        std::cout << "Position: " << x << ", " << y << " Value: " << grid(x / tileSize, y / tileSize) << '\n';
        x = position.x;
        y = position.y;
    }
}

