//
// Created by ginko on 20/05/24.
//
#include <fstream>
#include <iostream>
#include <string>


#include "toml++/toml.hpp"

const char *inputFile = "/home/ginko/jems-project/aptomodo/config/attributes.toml";
const char *outputFile = "/home/ginko/jems-project/aptomodo/includes/parameters.cpp";


int main() {
    // Load toml
    toml::table config;
    try {
        config = toml::parse_file(inputFile);
        std::cout << config << "\n";
    }
    catch (const toml::parse_error &err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
        return 1;
    }
    std::cerr << "Toml loaded" << "\n";

    // Read toml
    toml::array& startValues = *config.get_as<toml::array>("start_values");
    toml::array& multipliers = *config.get_as<toml::array>("multipliers");
    std::cerr << "Toml read" << "\n";

    // Open output file
    std::ofstream output;
    output.open(outputFile);
    std::cerr << "Output file opened" << "\n";

//    if (!output.is_open()) {
//        std::cerr << "Failed to open output file: " << outputFile << "\n";
//        return 1;
//    }
    output << "#include <array>\n";
    output << "#include \"parameters.h\"\n";
    output << "namespace parameters {\n";

    output << "std::array<int, 18> subAttrAtStart = {\n";
    for (auto&& v: startValues ){
        output << v.value_or(0) << ",\n";
    }
    output << "};\n";
    output << "\n";

    output << "std::array<float, 18> subAttrMultipliers = {\n";
    for (auto&& v: multipliers ){
        output << *v.as_floating_point() << "f,\n";
    }
    output << "};\n";
    
    output << "int expByLevel = " << config["expByLevel"] <<  ";\n";
    output << "int pointsByLevel = " << config["pointsByLevel"] <<  ";\n";
    output << "int pointsByAttr = " << config["pointsByAttr"] <<  ";\n";
    output << "int pointsAtStart = " << config["pointsAtStart"] <<  ";\n";

    output << "};\n";
    output << "\n";

    output.close();


    std::cerr << "Header file generated successfully.\n";

    return 0;
}
