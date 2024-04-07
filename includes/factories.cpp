//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <raylib.h>
#include <random>
#include "factories.h"
#include "constants.h"
#include "levels.h"





entt::entity spawnEnemy(entt::registry &registry, Position position, Level::Variables &level) {
    static int id = 0;
    entt::entity enemy = registry.create();
    registry.emplace<Radius>(enemy, level.radius);
    registry.emplace<Enemy>(enemy);
    registry.emplace<Living>(enemy);
    registry.emplace<ID>(enemy, id++);
    registry.emplace<Speed>(enemy, 5.0f);
    registry.emplace<Health>(enemy, 100, 100);
    registry.emplace<Position>(enemy, position.x, position.y);

    return enemy;
}

entt::entity spawnRandomEnemy(entt::registry &registry, Level::Variables &level) {
    Position randomPos = {static_cast<float>(rng::uniform(rng::seed)),
                          static_cast<float> (rng::uniform(rng::seed))};
    return spawnEnemy(registry, randomPos, level);
}

entt::entity spawnPlayer(entt::registry &registry, GameScene *scene, Level::Variables &level) {
    Texture2D texture = LoadTexture(scene->getTexturePath("Player").c_str()); // TODO: remember to unload if needed
    Animation::Map animationMap = scene->getAnimationMap("Player");

    entt::entity player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Living>(player);
    registry.emplace<Radius>(player, level.radius);
    registry.emplace<Texture>(player, texture);
    registry.emplace<Animation::Map>(player, animationMap);
    registry.emplace<Weapon>(player, level.sword);
    registry.emplace<Health>(player, 100, 100);
    registry.emplace<Position>(player, 500.0f, 500.0f);
    registry.emplace<PlayerState>(player, State::normal);
    registry.emplace<Pain>(player, level.pain);

    return player;
}

Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {0, 0};
    camera.offset = {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    return camera;
}

