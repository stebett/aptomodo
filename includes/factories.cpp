//
// Created by ginko on 02/03/24.
//

#include "factories.h"

#include <components.h>
#include "constants.h"
#include "attributes.h"
#include "enemyType.h"
#include "items.h"
#include "ai/strategies.h"
#include "managers/assets.h"
#include "systems/physics.h"
#include "managers/game.h"

// TODO Make a common factory, and define components used here in factories.h

entt::entity spawnLiving() {
    const entt::entity e = Game::registry.create();
    Game::registry.emplace<Living>(e);
    Game::registry.emplace<Radius>(e, 10.0f);
    Game::registry.emplace<Speed>(e, 3.0f);
    Game::registry.emplace<AttackTimer>(e);
    Game::registry.emplace<AttackSpeed>(e, 0.3f);
    Game::registry.emplace<Damage>(e, 5.0f);
    Game::registry.emplace<AttackRange>(e, 3 * Const::tileSize);
    Game::registry.emplace<Pushback>(e, 1.0f);
    Game::registry.emplace<Spread>(e, 10.0f);
    Game::registry.emplace<LookAngle>(e, 0.0f);

    return e;
}


entt::entity spawnEnemyByName(Vector2 position, const std::string &enemyName) {
    const EnemyType &stats = Assets::GetEnemiesData().getType(enemyName);
    static int id = 0;
    // TODO this should be hashed or something, so we don't have overlap if we add new spawns elsewhere, or stored in Game::ID
    const entt::entity e = Game::registry.create();
    Game::registry.emplace<Name>(e, stats.name);
    Game::registry.emplace<Grade>(e, stats.grade);
    Game::registry.emplace<Radius>(e, stats.radius);
    Game::registry.emplace<Speed>(e, stats.speed);
    Game::registry.emplace<AttackSpeed>(e, stats.attackSpeed);
    Game::registry.emplace<Damage>(e, stats.damage);
    Game::registry.emplace<AttackRange>(e, stats.attackRange);
    Game::registry.emplace<Spread>(e, stats.attackSpread);
    Game::registry.emplace<ColorBB>(e, RED);
    Game::registry.emplace<Health>(e, stats.health);
    Game::registry.emplace<Experience>(e, stats.experience);


    Game::registry.emplace<Path>(e);
    Game::registry.emplace<Position>(e, position);
    Game::registry.emplace<Enemy>(e);
    Game::registry.emplace<Chasing>(e);
    auto &chasing = Game::registry.get<Chasing>(e);
    chasing.timer.StartBehind(100);;
    Game::registry.emplace<ID>(e, id++);
    Game::registry.emplace<Strategy::Strategy>(e, std::make_unique<Strategy::Melee>(e));
    //TODO This leaks (how?)
    Game::registry.emplace<Living>(e);
    Game::registry.emplace<AttackTimer>(e);
    Game::registry.emplace<LookAngle>(e, 0.0f);
    auto bodyId = Physics::CreateDynamicCircularBody(e, position, stats.radius, Physics::Enemy);
    Game::registry.emplace<b2BodyId>(e, bodyId);
    return e;
};


entt::entity spawnPlayer(Vector2 position) {
    const entt::entity e = spawnLiving();
    Game::registry.emplace<Player>(e);
    Game::registry.emplace<ToRender>(e);
    Game::registry.emplace<ToSimulate>(e);
    Game::registry.emplace<Position>(e, position);
    Game::registry.emplace<ColorBB>(e, BLUE);
    auto attr = Attributes();
    Game::registry.emplace<Attributes>(e, attr);
    Game::registry.emplace<Health>(e, attr.getMultiplied(AttributeConstants::health));
    Game::registry.emplace<Experience>(e, 0);
    auto bodyId = Physics::CreateDynamicCircularBody(e, position, 10.0f, Physics::Player);
    Game::registry.emplace<b2BodyId>(e, bodyId);
    return e;
}


Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {Const::mapWidth / 2, Const::mapHeight / 2};
    camera.offset = {Const::screenWidth / 2, Const::screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
    return camera;
}

entt::entity spawnItemFromFile(Vector2 position, const std::string &name) {
    // TODO make this load amulets from file
    return entt::entity();
}


void spawnEntities(const std::vector<Level::Entity> &entities) {
    for (auto &[position, type, name]: entities) {
        if (type == "Enemy") spawnEnemyByName(position, name);
        if (type == "Item") spawnItemFromFile(position, name);
        if (type == "Player") spawnPlayer(position);
    }
}


// void spawnAmulet() {
//     Vector2 position{};
//     for (const auto &[label, pos]: LevelManager::GetEntitiesPositions()) {
//         if (label == "Amulet") position = pos;
//     }
//     AttributeConstants::Modifier modifier = {
//         AttributeConstants::strength, 2, AttributeConstants::ModifierOperator::ADD
//     };
//
//     entt::entity e = registry.create();
//     registry.emplace<Item>(e);
//     registry.emplace<Position>(e, position);
//     registry.emplace<AttributeConstants::Modifier>(e, modifier);
//     registry.emplace<Name>(e, "Amuleto de sto'cazzo");
// }
//
//
// void spawnAmulet2() {
//     Vector2 position{};
//     for (const auto &[label, pos]: LevelManager::GetEntitiesPositions()) {
//         if (label == "Amulet2") position = pos;
//     }
//     AttributeConstants::Modifier modifier = {
//         AttributeConstants::strength, 2, AttributeConstants::ModifierOperator::MUL
//     };
//
//     entt::entity e = registry.create();
//     registry.emplace<Item>(e);
//     registry.emplace<Position>(e, position);
//     registry.emplace<AttributeConstants::Modifier>(e, modifier);
//     registry.emplace<Name>(e, "Amuleto della mia minchia");
// }
