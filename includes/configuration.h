//
// Created by ginko on 20/05/24.
//

#ifndef ACEROLA_JAM0_CONFIGURATION_H
#define ACEROLA_JAM0_CONFIGURATION_H

#include <iostream>

#define TOML_EXCEPTIONS 0 // only necessary if you've left them enabled in your compiler
#include <toml++/toml.hpp>

int main()
{
    toml::parse_result result = toml::parse_file("configuration.toml");

    if (!result)
    {
        std::cerr << "Parsing failed:\n" << result.error() << "\n";
        return 1;
    }

    do_stuff_with_your_config(std::move(result).table()); // 'steal' the table from the result
    return 0;
}
#endif //ACEROLA_JAM0_CONFIGURATION_H
