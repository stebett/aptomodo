//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include "rendering.h"
#include "components.h"
#include "constants.h"


void draw(const entt::registry &registry, GameScene &scene) {
    scene.draw();

    auto playerView = registry.view<Player, Living, Radius, Position>();
    for (auto [entity, radius, position]: playerView.each()) {
        DrawCircle(position.x, position.y, radius.value, RED);
    }
    auto enemyView = registry.view<Enemy, Living, Radius, Position>();
    for (auto [entity, radius, position]: enemyView.each()) {
        DrawCircle(position.x, position.y, radius.value, BROWN);
    }
}

GameScene::GameScene() {
    ldtkProject = new ldtk::Project();
    std::cout << getAssetPath("project.ldtk") << "\n";
    ldtkProject->loadFromFile(getAssetPath("project.ldtk"));

    ldtkWorld = &ldtkProject->getWorld();

    current_level = -1;
    set_selected_level(0);
}


GameScene::~GameScene() {
    delete ldtkProject;
    delete ldtkWorld;

    ldtkProject = nullptr;

    UnloadTexture(renderedLevelTexture);
    UnloadTexture(currentTilesetTexture);
}

void GameScene::draw() {
    ClearBackground(RAYWHITE);
    DrawTextureRec(renderedLevelTexture,
                   {0, 0, (float) renderedLevelTexture.width, (float) -renderedLevelTexture.height},
                   {0, 0}, WHITE);

}

void GameScene::set_selected_level(int lvl) {
    // unload current tileset texture if necessary
    if (current_level >= 0) {
        UnloadTexture(currentTilesetTexture);
    }

    current_level = lvl;
    currentLdtkLevel = &ldtkWorld->getLevel(current_level);

    auto levelSize = currentLdtkLevel->size;
    auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);

    BeginTextureMode(renderTexture);

    // draw all tileset layers
    for (auto &&layer: currentLdtkLevel->allLayers()) {
        if (layer.hasTileset()) {
            currentTilesetTexture = LoadTexture((getAssetPath(layer.getTileset().path)).c_str());
            // if it is a tile layer then draw every tile to the frame buffer
            for (auto &&tile: layer.allTiles()) {
                auto source_pos = tile.getTextureRect();
                auto tile_size = float(layer.getTileset().tile_size);

                Rectangle source_rect = {
                        .x = float(source_pos.x),
                        .y = float(source_pos.y),
                        .width = tile.flipX ? -tile_size : tile_size,
                        .height = tile.flipY ? -tile_size : tile_size,
                };

                Vector2 target_pos = {
                        (float) tile.getPosition().x,
                        (float) tile.getPosition().y,
                };

                DrawTextureRec(currentTilesetTexture, source_rect, target_pos, WHITE);
            }
        }
    }

    EndTextureMode();
    renderedLevelTexture = renderTexture.texture;

}