//
// Created by ginko on 20/05/24.
//

#ifndef APTOMODO_PARAMETERS_H
#define APTOMODO_PARAMETERS_H

#include <array>
#include <attributesConstants.h>
#include <raylib.h>
#include <unordered_map>
#include <string>
#include "toml++/toml.hpp"

constexpr char const * attributeParametersPath{"../config/attributes.toml"};

struct AttributeParameters {
    int expByLevel;
    int pointsByLevel;
    int pointsByAttr;
    int pointsAtStart;
    std::array<float, 18> subAttrMultipliers{};
    std::array<float, 18> subAttrAtStart{};

    AttributeParameters() = default;
    explicit AttributeParameters(toml::table);
    toml::table writeTable();
};

/*
 * The purpose of this class is to load from a file:
 * - subattributes starting value
 * - subattributes multipliers
 * These need to be accessible by all functions that want to calculate a value based on subattributes
 * We can keep this global for now, but probably they can just stay in a namespace, with loading and saving functions publicy accessible
 */
namespace Params {

    static AttributeParameters attributes;
    static toml::table config;

    void SaveAttributeParameters();
    void LoadAttributeParameters();
    void Instantiate();
    float& Multiplier(AttributeConstants::SubAttributeName name);
    float MultiplierOriginal(AttributeConstants::SubAttributeName name);
    float& StartValue(AttributeConstants::SubAttributeName name);
    float StartValueOriginal(AttributeConstants::SubAttributeName name);
    int GetExpByLevel();
    int GetPointsByLevel();
    int GetPointsByAttr();
    int GetPointsAtStart();
    int GetExpByLevelOriginal();
    int GetPointsByLevelOriginal();
    int GetPointsByAttrOriginal();
    int GetPointsAtStartOriginal();


};

#endif //APTOMODO_PARAMETERS_H
