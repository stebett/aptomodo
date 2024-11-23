//
// Created by ginko on 02/03/24.
//

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
    registry.emplace<Health>(e, 50);
    registry.emplace<Experience>(e, 50);
    registry.emplace<Strategy::Strategy>(e, std::make_unique<Strategy::Melee>(registry, e)); //TODO need to be sure that this gets deleted
    return e;
}

struct EnemyStats {
    Name name;
    Grade grade;
    Radius radius;
    Speed speed;
    AttackSpeed attackSpeed;
    Damage damage;
    AttackRange attackRange;
    Spread attackSpread;
    ColorBB color;
    Health health;
    Experience experience;
    std::string attributesPath;
    std::string texturePath;

};

struct EnemyDataFile {
    std::string path {"../config/enemies.csv"};
    std::vector<std::string> headers;
    std::vector<EnemyStats> enemyStats;

    bool loadCSV(const std::string &filename);
};

bool EnemyDataFile::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string header;
        while (std::getline(ss, header, ',')) {
            headers.push_back(header);
        }
    }

    // Read the data rows
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        EnemyStats stats {};

        for (size_t i = 0; i < headers.size(); ++i) {
            std::getline(ss, cell, ',');

            // Assign data based on header
            if (headers[i] == "Name") {
                stats.name.value = cell;
            } else if (headers[i] == "Grade") {
                stats.grade.value = std::stoi(cell);
            } else if (headers[i] == "Radius") {
                stats.radius.value = std::stof(cell);
            } else if (headers[i] == "Speed") {
                stats.speed = std::stof(cell);
            } else if (headers[i] == "AttackSpeed") {
                stats.attackSpeed.value = std::stof(cell);
            } else if (headers[i] == "Damage") {
                stats.damage.value = std::stof(cell);
            } else if (headers[i] == "AttackRange") {
                stats.attackRange.value = std::stof(cell);
            } else if (headers[i] == "AttackSpread") {
                stats.attackSpread.value = std::stof(cell);
            } else if (headers[i] == "ColorBB") {
                stats.color.color = colorMap.at(cell);
            } else if (headers[i] == "Health") {
                stats.health = Health(std::stof(cell));
            } else if (headers[i] == "ExperienceGiven") {
                stats.experience.value = std::stoi(cell);
            // } else if (headers[i] == "Strategy") {
            } else if (headers[i] == "AttributesPath") {
                stats.attributesPath = cell;
            } else if (headers[i] == "TexturePath") {
                stats.texturePath = cell;
            }
        }
        enemyStats.push_back(stats);
    }

    file.close();
    return true;
}


entt::entity spawnEnemyFromFile(entt::registry &registry, Position position) {
    static int id = 0;
    const entt::entity e = registry.create();
    EnemyDataFile data {};
    data.loadCSV(data.path);
    for (const auto &stats: data.enemyStats) {
        registry.emplace<Name>(e,stats.name);
        registry.emplace<Grade>(e,stats.grade);
        registry.emplace<Radius>(e,stats.radius);
        registry.emplace<Speed>(e,stats.speed);
        registry.emplace<AttackSpeed>(e,stats.attackSpeed);
        registry.emplace<Damage>(e,stats.damage);
        registry.emplace<AttackRange>(e,stats.attackRange);
        registry.emplace<Spread>(e,stats.attackSpread);
        registry.emplace<ColorBB>(e,stats.color);
        registry.emplace<Health>(e,stats.health);
        registry.emplace<Experience>(e,stats.experience);
    }
    registry.emplace<Path>(e);
    registry.emplace<Position>(e, position);
    registry.emplace<Enemy>(e);
    registry.emplace<Chasing>(e);
    auto &chasing = registry.get<Chasing>(e);
    chasing.timer.StartBehind(100);;
    registry.emplace<ID>(e, id++);
    registry.emplace<Strategy::Strategy>(e, std::make_unique<Strategy::Melee>(registry, e)); //TODO need to be sure that this gets deleted
    registry.emplace<Living>(e);
    registry.emplace<AttackTimer>(e);
    registry.emplace<LookAngle>(e, 0.0f);
    return e;
};

entt::entity spawnRandomEnemy(entt::registry &registry) {
    const Position randomPos = {static_cast<float>(rng::uniform(rng::seed)),
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
    registry.emplace<Health>(e, attr.getMultiplied(AttributeConstants::health));
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
    for (const auto& [label, position]:  LevelManager::GetEnemiesPositions()) {
        spawnEnemy(registry, position);
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
