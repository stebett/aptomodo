//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <raylib.h>
#include <random>
#include "factories.h"
#include "constants.h"


entt::entity spawnEnemy(entt::registry &registry, Position position) {
    static int id = 0;
    entt::entity enemy = registry.create();
    registry.emplace<Radius>(enemy, 5.0f);
    registry.emplace<Spread>(enemy, 10.0f);
    registry.emplace<Enemy>(enemy);
    registry.emplace<Living>(enemy);
    registry.emplace<ID>(enemy, id++);
    registry.emplace<Speed>(enemy, 5.0f);
    registry.emplace<Health>(enemy, 100, 100);
    registry.emplace<Position>(enemy, position.x, position.y);
    registry.emplace<PhysicalResistance>(enemy, 0.0f);
    registry.emplace<MagicalResistance>(enemy, 0.0f);
    registry.emplace<Stamina>(enemy, 0.0f);
    registry.emplace<TimeLastAttack>(enemy, 0.0f);
    registry.emplace<AttackSpeed>(enemy, 2.0f);
    registry.emplace<Damage>(enemy, 1.0f);
    registry.emplace<AttackRange>(enemy, 3 * tileSize);
    registry.emplace<Pushback>(enemy, 1.0f);
    return enemy;
}

entt::entity spawnRandomEnemy(entt::registry &registry) {
    Position randomPos = {static_cast<float>(rng::uniform(rng::seed)),
                          static_cast<float> (rng::uniform(rng::seed))};
    return spawnEnemy(registry, randomPos);
}

entt::entity spawnPlayer(entt::registry &registry, Position position, GameScene *scene) {
    Texture2D texture = LoadTexture(scene->getTexturePath("Player").c_str()); // TODO: remember to unload if needed
    Animation::Map animationMap = scene->getAnimationMap("Player");
    entt::entity player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Living>(player);
    registry.emplace<Spread>(player, 15.0f);
    registry.emplace<Texture>(player, texture);
    registry.emplace<Animation::Map>(player, animationMap);
    registry.emplace<Health>(player, 100, 100);
    registry.emplace<Radius>(player, 10.0f);
    registry.emplace<PhysicalResistance>(player, 0.0f);
    registry.emplace<MagicalResistance>(player, 0.0f);
    registry.emplace<Stamina>(player, 0.0f);
    registry.emplace<TimeLastAttack>(player, 0.0f);
    registry.emplace<AttackSpeed>(player, 2.0f);
    registry.emplace<Damage>(player, 1.0f);
    registry.emplace<AttackRange>(player, 25.0f);
    registry.emplace<Pushback>(player, 1.0f);
    registry.emplace<Position>(player, position);

    return player;
}

Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {mapWidth / 2, mapHeight / 2};
    camera.offset = {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    return camera;
}

