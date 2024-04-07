//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_CONSTANTS_H
#define ACEROLA_JAM0_CONSTANTS_H


#include <random>
#include "components.h"
#include <eigen3/Eigen/Dense>

const int screenWidth = 1920;
const int screenHeight = 1080;
const int mapWidth = 1920;
const int mapHeight = 1080;
const int tileSize = 16;
const float radToDeg = (180.0 / 3.141592653589793238463);
const float degToRad = (3.141592653589793238463 / 180.0);
const int updateRate { 10 };
const int playerUpdateRate { 10 };

namespace rng {
    static std::random_device randomDevice;
    static std::default_random_engine seed(randomDevice());
    static std::uniform_int_distribution<int> uniform(1, 500);
}

inline std::string getAssetPath(const std::string& assetName)
{
    return ASSETS_PATH "" + assetName;
}

using Map = Eigen::Matrix<int, mapWidth / tileSize + 1, mapHeight / tileSize + 1>;



#endif //ACEROLA_JAM0_CONSTANTS_H
