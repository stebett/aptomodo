//
// Created by ginko on 02/03/24.
//
// Credits to https://github.com/tupini07/raylib-cpp-cmake-template

#ifndef ACEROLA_JAM0_RENDERING_H
#define ACEROLA_JAM0_RENDERING_H

#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <raylib.h>
#include <entt/entity/registry.hpp>
#include "components.h"
#include "gui.h"
#include "constants.h"


class GameScene {

    int current_level;

    entt::registry& m_registry;

    ldtk::Project *ldtkProject{};
    const ldtk::World *ldtkWorld{};
    const ldtk::Level *currentLdtkLevel{};

    Texture2D currentTilesetTexture;
    Texture2D renderedLevelTexture;

public:
    Map grid;

    GameScene(entt::registry &registry);

    ~GameScene();

    void setLevel(entt::registry &registry, int level);
    void draw();
    std::string getTexturePath(const std::string& tileset);
    Animation::Map getAnimationMap(const std::string& ldtkEnum);
};

void draw(const entt::registry &registry, GameScene *scene, unsigned int frame);

void updateCamera(Camera2D &camera, Position &playerPosition);


#endif //ACEROLA_JAM0_RENDERING_H
