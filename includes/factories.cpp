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

// TODO Make a common factory, and define components used here in factories.h

entt::entity spawnLiving(entt::registry &registry) {
    const entt::entity e = registry.create();
    registry.emplace<Living>(e);
    registry.emplace<Radius>(e, 10.0f);
    registry.emplace<Speed>(e, 3.0f);
    registry.emplace<AttackTimer>(e);
    registry.emplace<AttackSpeed>(e, 0.3f);
    registry.emplace<Damage>(e, 5.0f);
    registry.emplace<AttackRange>(e, 3 * tileSize);
    registry.emplace<Pushback>(e, 1.0f);
    registry.emplace<Spread>(e, 10.0f);
    registry.emplace<LookAngle>(e, 0.0f);

    return e;
}


entt::entity spawnEnemyFromFile(entt::registry &registry, Position position, const std::string &enemyName) {
    const EnemyType &stats = Assets::GetEnemiesData().getType(enemyName);
    static int id = 0;
    // TODO this should be hashed or something, so we don't have overlap if we add new spawns elsewhere, or stored in Game::ID
    const entt::entity e = registry.create();
    registry.emplace<Name>(e, stats.name);
    registry.emplace<Grade>(e, stats.grade);
    registry.emplace<Radius>(e, stats.radius);
    registry.emplace<Speed>(e, stats.speed);
    registry.emplace<AttackSpeed>(e, stats.attackSpeed);
    registry.emplace<Damage>(e, stats.damage);
    registry.emplace<AttackRange>(e, stats.attackRange);
    registry.emplace<Spread>(e, stats.attackSpread);
    registry.emplace<ColorBB>(e, stats.color);
    registry.emplace<Health>(e, stats.health);
    registry.emplace<Experience>(e, stats.experience);


    registry.emplace<Path>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<Enemy>(e);
    registry.emplace<Chasing>(e);
    auto &chasing = registry.get<Chasing>(e);
    chasing.timer.StartBehind(100);;
    registry.emplace<ID>(e, id++);
    registry.emplace<Strategy::Strategy>(e, std::make_unique<Strategy::Melee>(registry, e));
    //TODO need to be sure that this gets deleted
    registry.emplace<Living>(e);
    registry.emplace<AttackTimer>(e);
    registry.emplace<LookAngle>(e, 0.0f);
    return e;
};


entt::entity spawnPlayer(entt::registry &registry, Vector2 position) {
    const entt::entity e = spawnLiving(registry);
    registry.emplace<Player>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<ColorBB>(e, BLUE);
    auto attr = Attributes();
    registry.emplace<Attributes>(e, attr);
    registry.emplace<Health>(e, attr.getMultiplied(AttributeConstants::health));
    registry.emplace<Experience>(e, 0);
    return e;
}


Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {mapWidth / 2, mapHeight / 2};
    camera.offset = {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
    return camera;
}

entt::entity spawnItemFromFile(entt::registry &registry, Vector2 position, const std::string &name) {
    // TODO make this load amulets from file
    return entt::entity();
}


void spawnEntities(entt::registry &registry, const std::vector<Level::Entity>& entities) {
    for (auto& [position, type, name]: entities) {
        if (type == "Enemy") spawnEnemyFromFile(registry, position, name);
        if (type == "Item") spawnItemFromFile(registry, position, name);
        if (type == "Player") spawnPlayer(registry, position);
    }
}




// void spawnAmulet(entt::registry &registry) {
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
// void spawnAmulet2(entt::registry &registry) {
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
