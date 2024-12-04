//
// Created by ginko on 11/24/24.
//

#include "enemyType.h"

#include <constants.h>

bool EnemyDataFile::loadCSV(const std::string &filename) {
    auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(CONFIG_PATH) /  std::filesystem::path(filename);
    SPDLOG_INFO("Config trying to parse: ");
    SPDLOG_INFO(path);

    std::ifstream file(path);
    if (!file.is_open()) {
        SPDLOG_ERROR("[ENEMIES]:  Could not open enemy type file");
        return false;
    }
    SPDLOG_INFO("[ENEMIES]:   Reading type file");

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
        EnemyType stats{};

        for (size_t i = 0; i < headers.size(); ++i) {
            std::getline(ss, cell, ',');
            if (headers[i] == "Name") {
                stats.name = cell;
            } else if (headers[i] == "Grade") {
                stats.grade = std::stoi(cell);
            } else if (headers[i] == "Radius") {
                stats.radius = std::stof(cell);
            } else if (headers[i] == "Speed") {
                stats.speed = std::stof(cell);
            } else if (headers[i] == "AttackSpeed") {
                stats.attackSpeed = std::stof(cell);
            } else if (headers[i] == "Damage") {
                stats.damage = std::stof(cell);
            } else if (headers[i] == "AttackRange") {
                stats.attackRange = std::stof(cell);
            } else if (headers[i] == "AttackSpread") {
                stats.attackSpread = std::stof(cell);
            } else if (headers[i] == "ColorBB") {
                stats.color = colorMap.at(cell);
            } else if (headers[i] == "Health") {
                stats.health = std::stof(cell);
            } else if (headers[i] == "ExperienceGiven") {
                stats.experience = std::stoi(cell);
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

EnemyType& EnemyDataFile::getType(const std::string & name) {
    for (auto & enemyType: enemyStats) {
        if (enemyType.name == name) {
            return enemyType;
        }
    }
    SPDLOG_WARN("[ENEMIES]: did not find enemy of type");

    return enemyStats[0];
}

