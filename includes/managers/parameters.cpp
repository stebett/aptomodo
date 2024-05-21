// Created by ginko on 20/05/24.
//
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include "parameters.h"


Params *Params::instance;
AttributeParameters Params::attributes;
AttributeParameters Params::attributesOriginal;


void Params::LoadAttributeParameters() {
    toml::table config;
    try {
        config = toml::parse_file(attributeParametersPath);
    }
    catch (const toml::parse_error &err) {
        std::cerr << "WARNING: PARAMETERS: Parsing failed:\n" << err << "\n";
        return;
    }
    std::cout << "INFO: PARAMETERS: Attribute file loaded successfully" << "\n";

    attributes = AttributeParameters(config);
    attributesOriginal = attributes;
}


Params &Params::Instance() { return *instance; }

void Params::SaveAttributeParameters() {
    attributesOriginal = attributes;
    auto table = attributes.writeTable();
    std::ofstream output;
    output.open(attributeParametersPath);
    std::cout << "INFO: PARAMETERS: file opened for saving" << "\n";
    output << table;
    std::cout << "INFO: PARAMETERS: parameters saved" << "\n";
    output.close();
    std::cout << "INFO: PARAMETERS: file closed" << "\n";

}

AttributeParameters::AttributeParameters(toml::table config) {
    fps = config["fps"].value_or(0);
    strategy = config["strategy"].value_or(0);
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
    toml::array result {};
    for (size_t i = 0; i < 18; ++i) {
        result.push_back(array[i]);
    }
    return result;
}

toml::table AttributeParameters::writeTable() {
    auto table = toml::table{
            {"fps",                fps},
            {"strategy",           strategy},
            {"expByLevel",         expByLevel},
            {"pointsByLevel",      pointsByLevel},
            {"pointsByAttr",       pointsByAttr},
            {"pointsAtStart",      pointsAtStart},

            {"subAttrMultipliers", serializeArray(subAttrMultipliers)},
            {"subAttrAtStart",     serializeArray(subAttrAtStart)},
    };
    return table;
}
