//
// Created by ginko on 02/03/24.
//

#include "pch.h"
#include "factories.h"
#include "constants.h"
#include "managers/levelManager.h"
#include "attributes.h"
#include "items.h"
#include "ai/strategies.h"

// TODO Make a common factory, and define components used here in factories.h

entt::entity spawnLiving(entt::registry &registry) {
    const entt::entity e = registry.create();
    registry.emplace<Living>(e);
    registry.emplace<Radius>(e, 10.0f);
    registry.emplace<Speed>(e, 3.0f);
    registry.emplace<PhysicalResistance>(e, 0.0f);
    registry.emplace<MagicalResistance>(e, 0.0f);
    registry.emplace<Stamina>(e, 0.0f);
    registry.emplace<AttackTimer>(e);
    registry.emplace<AttackSpeed>(e, 0.3f);
    registry.emplace<Damage>(e, 5.0f);
    registry.emplace<AttackRange>(e, 3 * tileSize);
    registry.emplace<Pushback>(e, 1.0f);
    registry.emplace<Spread>(e, 10.0f);
    registry.emplace<LookAngle>(e, 0.0f);

    return e;
}

entt::entity spawnEnemy(entt::registry &registry, Vector2 position) {
    static int id = 0;
    entt::entity e = spawnLiving(registry);
//    if (id > 0) { return enemy; }
    registry.emplace<Path>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<ColorBB>(e, RED);
    registry.emplace<Enemy>(e);
    registry.emplace<Chasing>(e);
    auto &chasing = registry.get<Chasing>(e);
    chasing.timer.StartBehind(100);;
    registry.emplace<ID>(e, id++);
    auto attr = Attributes();
    registry.emplace<Attributes>(e, attr);
    registry.emplace<Health>(e, attr.getMultiplied(AttributeConstants::health), attr.getMultiplied(AttributeConstants::health));
    registry.emplace<Experience>(e, 50);
    registry.emplace<Strategy::Strategy>(e, std::make_unique<Strategy::Melee>(registry, e)); //TODO need to be sure that this gets deleted
    return e;
}

entt::entity spawnRandomEnemy(entt::registry &registry) {
    Position randomPos = {static_cast<float>(rng::uniform(rng::seed)),
                          static_cast<float> (rng::uniform(rng::seed))};
    return spawnEnemy(registry, randomPos);
}

entt::entity spawnPlayer(entt::registry &registry, Vector2 position) {
    entt::entity e = spawnLiving(registry);
    registry.emplace<Player>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<ColorBB>(e, BLUE);
    auto attr = Attributes();
    registry.emplace<Attributes>(e, attr);
    registry.emplace<Health>(e, attr.getMultiplied(AttributeConstants::health), attr.getMultiplied(AttributeConstants::health));
    registry.emplace<Experience>(e, 0);
    return e;
}

entt::entity spawnPlayer(entt::registry &registry) {
    auto entitiesPositions = LevelManager::GetEntitiesPositions();
    for (const auto& [label, position]: entitiesPositions) {
        if (label == "Player") return spawnPlayer(registry, position);
    }
    return {};
}

void spawnEnemies(entt::registry &registry) {
    auto entitiesPositions = LevelManager::GetEntitiesPositions();
    for (const auto& [label, position]: entitiesPositions) {
        if (label == "Enemy") spawnEnemy(registry, position);
    }
}

Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {mapWidth / 2, mapHeight / 2};
    camera.offset = {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 4.0f;
    return camera;
}

void spawnAmulet(entt::registry& registry)
{
    Vector2 position {};
    for (const auto& [label, pos]: LevelManager::GetEntitiesPositions()) {
        if (label == "Amulet") position = pos;
    }
    AttributeConstants::Modifier modifier = {AttributeConstants::strength, 2, AttributeConstants::ModifierOperator::ADD};

    entt::entity e = registry.create();
    registry.emplace<Item>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<AttributeConstants::Modifier>(e, modifier);
    registry.emplace<Name>(e, "Amuleto de sto'cazzo");
}


void spawnAmulet2(entt::registry& registry)
{
    Vector2 position {};
    for (const auto& [label, pos]: LevelManager::GetEntitiesPositions()) {
        if (label == "Amulet2") position = pos;
    }
    AttributeConstants::Modifier modifier = {AttributeConstants::strength, 2, AttributeConstants::ModifierOperator::MUL};

    entt::entity e = registry.create();
    registry.emplace<Item>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<AttributeConstants::Modifier>(e, modifier);
    registry.emplace<Name>(e, "Amuleto della mia minchia");
}

void spawnItems(entt::registry& registry) {
    spawnAmulet(registry);
    spawnAmulet2(registry);
}
