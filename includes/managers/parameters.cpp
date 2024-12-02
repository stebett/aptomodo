// Created by ginko on 20/05/24.
//

#include "pch.h"
#include "parameters.h"

namespace Params {
    void LoadAttributeParameters() {
        try {
            config = toml::parse_file(attributeParametersPath);
        } catch (const toml::parse_error &err) {
            SPDLOG_WARN("[PARAMETERS]: Parsing failed: {}", err);
            return;
        }
        SPDLOG_INFO("PARAMETERS: Attribute file loaded successfully");

        attributes = AttributeParameters(config);
    }

    void Instantiate() {
        LoadAttributeParameters();
    }

    float &Multiplier(const AttributeConstants::SubAttributeName name) {
        return attributes.subAttrMultipliers[name];
    }

    float MultiplierOriginal(const AttributeConstants::SubAttributeName name) {
        return *config.get_as<toml::array>("subAttrMultipliers")->at(name).value<float>();
    }

    float &StartValue(const AttributeConstants::SubAttributeName name) {
        return attributes.subAttrAtStart[name];
    }

    float StartValueOriginal(const AttributeConstants::SubAttributeName name) {
        return *config.get_as<toml::array>("subAttrAtStart")->at(name).value<float>();
    }

    int GetExpByLevel() { return attributes.expByLevel; }

    int GetPointsByLevel() { return attributes.pointsByLevel; }

    int GetPointsByAttr() { return attributes.pointsByAttr; }

    int GetPointsAtStart() { return attributes.pointsAtStart; }

    int GetExpByLevelOriginal() { return config["expByLevel"].value_or(0); }

    int GetPointsByLevelOriginal() {
        return config["pointsByLevel"].value_or(0);
    }

    int GetPointsByAttrOriginal() { return config["pointsByAttr"].value_or(0); }

    int GetPointsAtStartOriginal() { return config["pointsAtStart"].value_or(0); }

    void SaveAttributeParameters() {
        auto table = attributes.writeTable();
        std::ofstream output;
        output.open(attributeParametersPath);
        SPDLOG_INFO("[PARAMETERS]: file opened for saving");
        output << table;
        SPDLOG_INFO("[PARAMETERS]: parameters saved");
        output.close();
        SPDLOG_INFO("[PARAMETERS]: file closed");
        LoadAttributeParameters();
    }
}

AttributeParameters::AttributeParameters(toml::table config) {
    expByLevel = config["expByLevel"].value_or(0);
    pointsByLevel = config["pointsByLevel"].value_or(0);
    pointsByAttr = config["pointsByAttr"].value_or(0);
    pointsAtStart = config["pointsAtStart"].value_or(0);

    subAttrMultipliers = [&] {
        std::array<float, 18> target{};
        for (size_t i = 0; i < 18; ++i) {
            target[i] = *config.get_as<toml::array>("subAttrMultipliers")->at(i).value<float>();
        }
        return target;
    }();

    subAttrAtStart = [&] {
        std::array<float, 18> target{};
        for (size_t i = 0; i < 18; ++i) {
            target[i] = *config.get_as<toml::array>("subAttrAtStart")->at(i).value<float>();
        }
        return target;
    }();
}

toml::array serializeArray(std::array<float, 18> array) {
    toml::array result{};
    for (size_t i = 0; i < 18; ++i) {
        result.push_back(array[i]);
    }
    return result;
}

toml::table AttributeParameters::writeTable() {
    auto table = toml::table{
        {"expByLevel", expByLevel},
        {"pointsByLevel", pointsByLevel},
        {"pointsByAttr", pointsByAttr},
        {"pointsAtStart", pointsAtStart},

        {"subAttrMultipliers", serializeArray(subAttrMultipliers)},
        {"subAttrAtStart", serializeArray(subAttrAtStart)},
    };
    return table;
}
