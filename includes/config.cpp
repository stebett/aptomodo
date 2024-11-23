//
// Created by ginko on 16/04/24.
//

#include "config.h"

namespace config {
    bool show_astar_path = false;
    bool show_enemy_fov = false;
    bool show_enemy_window = false;
    bool show_bounding_box = true;
    bool show_enemy_texture = false;
    bool show_attacks = true;
    bool show_attr_window = false;
    bool show_inv_window = false;
    bool free_camera = false;
    bool draw_level_collisions = false;
    float enemySightRange = 130.0f;
    float enemySightRangeChasing = 250.0f;
    float enemyHearRange = 70.0f;
    float enemyHearRangeChasing = 100.0f;
    float enemyPatrolDistance = 90;

    int fps = 60;
    int enemy_walking_animation_fps = 8;
    int strategy = 0;


    std::array<float, 18> attrMultipliers{
        3.0f, // damagePhysical
        10.0f, // health
        10.0f, // resistancePhysical
        10.0f, // damageMagical
        10.0f, // mana
        10.0f, // resistanceMagical
        0.50f, // speed
        0.03f, // attackSpeed
        10.0f, // spellSpeed
        10.0f, // regenerationMana
        10.0f, // regenerationStamina
        10.0f, // resistanceStatus
        1.0f, // spread
        6.0f, // range
        10.0f, // dodgeRange
        10.0f, // visualRange
        10.0f, // critChance
        10.0f, // critMultiplier
    };
};

int Config::storedIntsIndex{0};
int Config::storedFloatsIndex{0};
int Config::storedBoolsIndex{0};
std::array<int, preStoredValues> Config::storedInts{};
std::array<float, preStoredValues> Config::storedFloats{};
std::array<bool, preStoredValues> Config::storedBools{};
std::unordered_map<std::string, int> Config::indexDict{};
toml::table Config::config{};

void Config::SaveAttributeParameters() {
}

void Config::LoadAttributeParameters() {
    try {
        config = toml::parse_file(configPath);
    } catch (const toml::parse_error &err) {
        std::cerr << "WARNING: CONFIG: Parsing failed:\n" << err << "\n";
        return;
    }
    std::cout << "INFO: CONFIG: File loaded successfully" << "\n";
}

int Config::GetInt(const std::string &name) {
    if (indexDict.contains(name)) {
        return storedInts[indexDict[name]];
    }
    return config[name].value_or(0);
}

int *Config::GetIntPtr(const std::string &name) {
    if (indexDict.contains(name)) {
        return &storedInts[indexDict[name]];
    }
    storedInts[storedIntsIndex] = GetInt(name);
    indexDict[name] = storedIntsIndex;
    storedIntsIndex++;
    assert(storedIntsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
    return &storedInts[storedIntsIndex - 1];
}

float Config::GetFloat(const std::string &name) {
    if (indexDict.contains(name)) {
        return storedFloats[indexDict[name]];
    }
    return config[name].value_or(0.0f);
}

float *Config::GetFloatPtr(const std::string &name) {
    if (indexDict.contains(name)) {
        return &storedFloats[indexDict[name]];
    }
    storedFloats[storedFloatsIndex] = GetFloat(name);
    indexDict[name] = storedFloatsIndex;
    storedFloatsIndex++;
    assert(storedFloatsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
    return &storedFloats[storedFloatsIndex - 1];
}

bool *Config::GetBoolPtr(const std::string &name) {
    if (indexDict.contains(name)) {
        return &storedBools[indexDict[name]];
    }
    storedBools[storedBoolsIndex] = GetBool(name);
    indexDict[name] = storedBoolsIndex;
    storedBoolsIndex++;
    assert(storedBoolsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
    return &storedBools[storedBoolsIndex - 1];
}


bool Config::GetBool(const std::string &name) {
    if (indexDict.contains(name)) {
        return storedBools[indexDict[name]];
    }
    return config[name].value_or(false);
}
