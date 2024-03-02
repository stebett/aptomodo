//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <raylib.h>
#include "factories.h"
#include "constants.h"


entt::entity spawnEnemy(entt::registry &registry, Position position, Level level) {
    entt::entity enemy = registry.create();
    switch (level) {
        case Level::One:
            registry.emplace<Radius>(enemy, 15);
            break;
        case Level::Two:
            registry.emplace<Radius>(enemy, 25);
            break;
        case Level::Three:
            registry.emplace<Radius>(enemy, 35);
            break;
        case Level::Four:
            registry.emplace<Radius>(enemy, 45);
            break;
        case Level::Five:
            registry.emplace<Radius>(enemy, 55);
            break;
    }
    registry.emplace<Enemy>(enemy);
    registry.emplace<Living>(enemy);
    registry.emplace<Health>(enemy, 100, 100);
    registry.emplace<Position>(enemy, position.x, position.y);
    return enemy;
}

entt::entity spawnPlayer(entt::registry &registry) {
    entt::entity player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Living>(player);
    registry.emplace<Radius>(player, 20);
    registry.emplace<Weapon>(player, sword);
    registry.emplace<Health>(player, 100, 100);
    registry.emplace<Position>(player, 500.0f, 500.0f);
    registry.emplace<PlayerState>(player, State::normal);
    registry.emplace<Pain>(player, 100.0f);

    return player;
}

Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
}

