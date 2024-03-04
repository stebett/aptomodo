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




class GameScene {

    int current_level;

    entt::registry& m_registry;

    ldtk::Project *ldtkProject{};
    const ldtk::World *ldtkWorld{};
    const ldtk::Level *currentLdtkLevel{};

    Texture2D currentTilesetTexture;
    Texture2D renderedLevelTexture;
public:
    GameScene(entt::registry &registry);

    ~GameScene();

    void set_selected_level(entt::registry &registry, int level);
    void draw();
};

void draw(const entt::registry &registry, GameScene &scene);



#endif //ACEROLA_JAM0_RENDERING_H
