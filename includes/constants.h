//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_CONSTANTS_H
#define ACEROLA_JAM0_CONSTANTS_H


#include "components.h"

const int screenWidth = 1920;
const int screenHeight = 1080;
const int mapWidth = 1920;
const int mapHeight = 1080;
const float radToDeg = (180.0 / 3.141592653589793238463);
const float degToRad = (3.141592653589793238463 / 180.0);

inline std::string getAssetPath(const std::string& assetName)
{
    return ASSETS_PATH "" + assetName;
}

#endif //ACEROLA_JAM0_CONSTANTS_H
