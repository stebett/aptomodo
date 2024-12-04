//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_CONSTANTS_H
#define APTOMODO_CONSTANTS_H


namespace Const {
    constexpr int screenWidth = 1920; // TODO allow resizing
    constexpr int screenHeight = 1080;
    constexpr int mapWidth = 1528;
    constexpr int mapHeight = 1136;
    constexpr int IntGridHeight = 71;
    constexpr int IntGridWidth = 96;
    constexpr int tileSize = 16;
}


namespace rng {
    static std::random_device randomDevice;
    static std::default_random_engine seed(randomDevice());
    static std::uniform_int_distribution<int> uniform(1, 500);
    static std::uniform_int_distribution<int> uniform_neg500_500(-500, 500);
}

inline std::string getAssetPath(const std::string &assetName) {
    return (std::filesystem::path(ROOT_PATH) / std::filesystem::path(ASSETS_PATH) / std::filesystem::path(assetName)).string();
}

const std::unordered_map<std::string, Color> colorMap = {
    {"lightgray", LIGHTGRAY},
    {"gray", GRAY},
    {"darkgray", DARKGRAY},
    {"yellow", YELLOW},
    {"gold", GOLD},
    {"orange", ORANGE},
    {"pink", PINK},
    {"red", RED},
    {"maroon", MAROON},
    {"green", GREEN},
    {"lime", LIME},
    {"darkgreen", DARKGREEN},
    {"skyblue", SKYBLUE},
    {"blue", BLUE},
    {"darkblue", DARKBLUE},
    {"purple", PURPLE},
    {"violet", VIOLET},
    {"darkpurple", DARKPURPLE},
    {"beige", BEIGE},
    {"brown", BROWN},
    {"darkbrown", DARKBROWN}
};


#endif //APTOMODO_CONSTANTS_H
