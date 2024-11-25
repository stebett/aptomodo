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

constexpr char const * attributeParametersPath{"../config/attributes.toml"};

struct AttributeParameters {
    int fps; // todo remove
    int strategy; // todo remove
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
class Params {
    static Params *instance;

public:
    static AttributeParameters attributes; // TODO instead of having 2 it can probably just load again just one value if it needs resetting?
    static AttributeParameters attributesOriginal;

    static void SaveAttributeParameters();
    static void LoadAttributeParameters();

    static Params &Instance(); // TODO Why in the world this needs to be a singleton

    static void Instantiate() {
        instance = new Params();
        LoadAttributeParameters();
    }


//    ~Parameters destroy unloads all
};

#endif //APTOMODO_PARAMETERS_H
