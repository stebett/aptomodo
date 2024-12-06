//
// Created by ginko on 11/24/24.
//

#include "enemyType.h"
#include "rapidcsv.h"
#include <constants.h>

const std::filesystem::path EnemyDataFile::path {std::filesystem::path(ROOT_PATH) / std::filesystem::path(CONFIG_PATH) / "enemies.csv"};


bool EnemyDataFile::loadCSV() {
    enemyStats.clear();
    try {
        SPDLOG_INFO("Enemies file trying to parse: {}", path);
        rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));
        SPDLOG_INFO("[ENEMIES]: Reading type file");
        size_t rowCount = doc.GetRowCount();
        for (size_t i = 0; i < rowCount; ++i) {
            EnemyType stats;

            stats.name = doc.GetCell<std::string>("Name", i);
            stats.grade = doc.GetCell<int>("Grade", i);
            stats.radius = doc.GetCell<float>("Radius", i);
            stats.speed = doc.GetCell<float>("Speed", i);
            stats.attackSpeed = doc.GetCell<float>("AttackSpeed", i);
            stats.damage = doc.GetCell<float>("Damage", i);
            stats.attackRange = doc.GetCell<float>("AttackRange", i);
            stats.attackSpread = doc.GetCell<float>("AttackSpread", i);
            stats.health = doc.GetCell<float>("Health", i);
            stats.experience = doc.GetCell<int>("ExperienceGiven", i);
            stats.attributesPath = doc.GetCell<std::string>("AttributesPath", i);
            stats.texturePath = doc.GetCell<std::string>("TexturePath", i);

            enemyStats.push_back(stats);
        }

        return true;
    } catch (const std::exception& e) {
        SPDLOG_ERROR("[ENEMIES]: Error loading enemy types: {}", e.what());
        return false;
    }
}


bool EnemyDataFile::saveCSV(const std::vector<EnemyType>& enemies) {
    try {
        std::vector<std::string> names, attributesPaths, texturePaths, colors;
        std::vector<int> grades, experiences;
        std::vector<float> radii, speeds, attackSpeeds, damages, attackRanges, attackSpreads, healths;

        for (const auto& enemy : enemies) {
            names.push_back(enemy.name);
            grades.push_back(enemy.grade);
            radii.push_back(enemy.radius);
            speeds.push_back(enemy.speed);
            attackSpeeds.push_back(enemy.attackSpeed);
            damages.push_back(enemy.damage);
            attackRanges.push_back(enemy.attackRange);
            attackSpreads.push_back(enemy.attackSpread);
            healths.push_back(enemy.health);
            experiences.push_back(enemy.experience);
            attributesPaths.push_back(enemy.attributesPath);
            texturePaths.push_back(enemy.texturePath);
        }

        rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));

        doc.SetColumn<std::string>("Name", names);
        doc.SetColumn<int>("Grade", grades);
        doc.SetColumn<float>("Radius", radii);
        doc.SetColumn<float>("Speed", speeds);
        doc.SetColumn<float>("AttackSpeed", attackSpeeds);
        doc.SetColumn<float>("Damage", damages);
        doc.SetColumn<float>("AttackRange", attackRanges);
        doc.SetColumn<float>("AttackSpread", attackSpreads);
        doc.SetColumn<float>("Health", healths);
        doc.SetColumn<int>("ExperienceGiven", experiences);
        doc.SetColumn<std::string>("AttributesPath", attributesPaths);
        doc.SetColumn<std::string>("TexturePath", texturePaths);

        doc.Save(path);

        SPDLOG_INFO("[ENEMIES]: File saved successfully to {}", path.string());
        return true;
    } catch (const std::exception& e) {
        SPDLOG_ERROR("[ENEMIES]: Failed to save file: {}", e.what());
        return false;
    }
}



EnemyType &EnemyDataFile::getType(const std::string &name) {
    for (auto &enemyType: enemyStats) {
        if (enemyType.name == name) {
            return enemyType;
        }
    }
    SPDLOG_WARN("[ENEMIES]: did not find enemy of type");

    return enemyStats[0];
}

