//
// Created by ginko on 20/05/24.
//

#ifndef APTOMODO_PARAMETERS_H
#define APTOMODO_PARAMETERS_H

#include <array>
#include <raylib.h>
#include <unordered_map>
#include <string>
#include "toml++/toml.hpp"

constexpr char const * attributeParametersPath{"attributes.toml"};

struct AttributeParameters {
    int fps;
    int strategy;
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

class Params {
    static Params *instance;

public:
    static AttributeParameters attributes;
    static AttributeParameters attributesOriginal;

    static void SaveAttributeParameters();
    static void LoadAttributeParameters();

    static Params &Instance();

    static void Instantiate() {
        instance = new Params();
        LoadAttributeParameters();
    }


//    ~Parameters destroy unloads all
};

#endif //APTOMODO_PARAMETERS_H
