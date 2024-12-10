//
// Created by ginko on 16/04/24.
//

#include "configManager.h"

int Config::storedIntsIndex{0};
int Config::storedFloatsIndex{0};
int Config::storedBoolsIndex{0};
std::array<int, preStoredValues> Config::storedInts{};
std::array<float, preStoredValues> Config::storedFloats{};
std::array<bool, preStoredValues> Config::storedBools{};
std::unordered_map<std::string, int> Config::indexDictInts{};
std::unordered_map<std::string, int> Config::indexDictFloats{};
std::unordered_map<std::string, int> Config::indexDictBools{};
toml::table Config::config{};

toml::table Config::writeTable() {
    auto table = toml::table{};
    for (auto [key, value]: indexDictBools) table.emplace(key, storedBools[value]);
    for (auto [key, value]: indexDictInts) table.emplace(key, storedInts[value]);
    for (auto [key, value]: indexDictFloats) table.emplace(key, storedFloats[value]);
    return table;
}

void Config::SaveAttributeParameters() {
    try {
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(CONFIG_PATH) /
                    std::filesystem::path(configPath);
        SPDLOG_INFO("Config trying to save: ");
        SPDLOG_INFO(path);
        std::ofstream output(path, std::ios::out);
        if (!output) {
            SPDLOG_ERROR("[PARAMETERS]: Failed to open file for saving");
            return;
        }
        SPDLOG_INFO("[PARAMETERS]: File opened for saving");
        output << writeTable();
        SPDLOG_INFO("[PARAMETERS]: Parameters saved");

    } catch (const std::exception &e) {
        SPDLOG_ERROR("[PARAMETERS]: Error saving parameters - {}", e.what());
    } catch (...) {
        SPDLOG_ERROR("[PARAMETERS]: Unknown error occurred during saving");
    }
    SPDLOG_INFO("[PARAMETERS]: File closed");
}


void Config::LoadAttributeParameters() {
    try {
        auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(CONFIG_PATH) /
                    std::filesystem::path(configPath);
        SPDLOG_INFO("Config trying to parse: ");
        SPDLOG_INFO(path);
        config = toml::parse_file(path.string());
        for (const auto &[key, value]: config) {
            if (value.is_integer()) {
                addInt(key.data());
            } else if (value.is_floating_point()) {
                addFloat(key.data());
            } else if (value.is_boolean()) {
                addBool(key.data());
            }
        }
    } catch (const toml::parse_error &err) {
        SPDLOG_ERROR("[CONFIG]: Parsing failed");
        return;
    }
    SPDLOG_INFO("[CONFIG]: File loaded successfully");
}


void Config::addBool(const std::string &name) {
    storedBools[storedBoolsIndex] = config[name].value_or(false);
    indexDictBools[name] = storedBoolsIndex;
    storedBoolsIndex++;
    assert(storedBoolsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
}

bool Config::GetBool(const std::string &name) {
    return storedBools[indexDictBools[name]];
}

bool *Config::GetBoolPtr(const std::string &name) {
    return &storedBools[indexDictBools[name]];
}

void Config::addInt(const std::string &name) {
    storedInts[storedIntsIndex] = config[name].value_or(0);
    indexDictInts[name] = storedIntsIndex;
    storedIntsIndex++;
    assert(storedIntsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
}

int Config::GetInt(const std::string &name) {
    return storedInts[indexDictInts[name]];
}

int *Config::GetIntPtr(const std::string &name) {
    return &storedInts[indexDictInts[name]];
}

void Config::addFloat(const std::string &name) {
    storedFloats[storedFloatsIndex] = config[name].value<float>().value();
    indexDictFloats[name] = storedFloatsIndex;
    storedFloatsIndex++;
    assert(storedFloatsIndex < preStoredValues && "[CONFIG] Not enough pre-stored values");
}

float Config::GetFloat(const std::string &name) {
    return storedFloats[indexDictFloats[name]];
}

float *Config::GetFloatPtr(const std::string &name) {
    return &storedFloats[indexDictFloats[name]];
}

void Config::Instantiate() {
    SPDLOG_INFO("Working dir: ", GetWorkingDirectory());
    SPDLOG_INFO("Instantiating Config");
    LoadAttributeParameters();
}
