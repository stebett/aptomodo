//
// Created by ginko on 17/04/24.
//

#ifndef ACEROLA_JAM0_LEVELMANAGER_H
#define ACEROLA_JAM0_LEVELMANAGER_H

#include <raylib.h>
#include <unordered_map>
#include <string>
#include <entt/entity/registry.hpp>
#include <LDtkLoader/Project.hpp>
#include "../constants.h"

class LevelManager {

    static const char *root;
    static LevelManager *instance;

    static int current_level;

    static ldtk::Project *ldtkProject;
    static const ldtk::World *ldtkWorld;
    static const ldtk::Level *currentLdtkLevel;

    static Texture2D currentTilesetTexture;
    static Texture2D renderedLevelTexture;

    static std::vector<std::pair<std::string, Vector2>> entitiesPositions;
    static std::hash<std::string> hasher;


public:
    static Map grid;

    ~LevelManager();

    static std::vector<std::pair<std::string, Vector2>> & GetEntitiesPositions();
    static void Draw(Camera2D &camera);

    static void SetLevel(int level);

    static LevelManager &Instance();

    static void Instantiate();
};


#endif //ACEROLA_JAM0_LEVELMANAGER_H



