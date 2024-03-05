//
// Created by ginko on 02/03/24.
//

#include "rendering.h"
#include "constants.h"
#include "factories.h"

Rectangle toRectangle(ldtk::Rect<int> rectangle)
{
    return {
            .x = float(rectangle.x),
            .y = float(rectangle.y),
            .width = float(rectangle.width),
            .height = float(rectangle.height),
    };
}

void draw(const entt::registry &registry, GameScene &scene, unsigned int frame) {
    scene.draw();

    auto playerView = registry.view<Player, Living, Texture, Animation::Map, Position>();
    for (auto [entity, texture, animationMap, position]: playerView.each()) {
//        DrawTexture(texture, position.x, position.y, WHITE);
//        DrawCircle(position.x, position.y, radius.value, RED);
        Rectangle sourceRec = animationMap.at(Animation::Stand)[(frame % playerUpdateRate) % animationMap.at(Animation::Stand).size()];
        DrawTextureRec(texture, sourceRec, {position.x, position.y}, WHITE);

    }
    auto enemyView = registry.view<Enemy, Living, Radius, Position>();
    for (auto [entity, radius, position]: enemyView.each()) {
        DrawCircle(position.x, position.y, radius.value, BROWN);
    }
}

GameScene::GameScene(entt::registry &registry) : m_registry(registry) {
    ldtkProject = new ldtk::Project();
    std::cout << getAssetPath("project.ldtk") << "\n";
    ldtkProject->loadFromFile(getAssetPath("project.ldtk"));

    ldtkWorld = &ldtkProject->getWorld();

    current_level = -1;
    setLevel(registry, 0);
}


GameScene::~GameScene() {
    delete ldtkProject;
    delete ldtkWorld;

    ldtkProject = nullptr;

    UnloadTexture(renderedLevelTexture);
    UnloadTexture(currentTilesetTexture);
//    m_registry.remove<Texture>();
}

void GameScene::draw() {
    ClearBackground(BLUE);
    DrawTextureRec(renderedLevelTexture,
                   {0, 0, (float) renderedLevelTexture.width, (float) -renderedLevelTexture.height},
                   {0, 0}, WHITE);

}

void GameScene::setLevel(entt::registry &registry, int level) {
    // unload current tileset texture if necessary
    if (current_level >= 0) {
        UnloadTexture(currentTilesetTexture);
    }

    currentLdtkLevel = &ldtkWorld->getLevel(level);

    auto levelSize = currentLdtkLevel->size;
    auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);

    BeginTextureMode(renderTexture);

    if (currentLdtkLevel->hasBgImage())
    {
        auto backgroundPath = currentLdtkLevel->getBgImage();
        auto backgroundTexture = LoadTexture(getAssetPath(backgroundPath.path.c_str()).c_str());
        SetTextureFilter(backgroundTexture, TEXTURE_FILTER_POINT);

        // tile background texture to cover the whole frame buffer
        for (int i = 0; i <= (mapWidth / backgroundTexture.width); i++)
        {
            for (int j = 0; j <= (mapHeight / backgroundTexture.height); j++)
            {
                DrawTextureV(backgroundTexture, {float(i * backgroundTexture.width), float(j * backgroundTexture.height)}, WHITE);
            }
        }
    }

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

//    for (auto &&entity: currentLdtkLevel->getLayer("Entities").allEntities()) {
//        if (entity.getName() == "Player") {
//            spawnPlayer(registry, entity, level);
//        }
//
//        if (entity.getName() == "Portal") {
//            float target_lvl = entity.getField<float>("level_destination").value();
//        }
//    }


    EndTextureMode();
    renderedLevelTexture = renderTexture.texture;

}

std::string GameScene::getTexturePath(const std::string& tileset) {
    return getAssetPath(ldtkWorld -> getTileset(tileset).path);
}

Animation::Map GameScene::getAnimationMap(const std::string &ldtkEnum) {
    auto &playerEnum = ldtkWorld -> getEnum(ldtkEnum);
    auto rec1 = toRectangle(playerEnum["Standing1"].getIconTextureRect());
    auto rec2 = toRectangle(playerEnum["Standing2"].getIconTextureRect());
    auto rec3 = toRectangle(playerEnum["Standing3"].getIconTextureRect());
    auto rec4 = toRectangle(playerEnum["Standing4"].getIconTextureRect());
    auto rec5 = toRectangle(playerEnum["Standing5"].getIconTextureRect());
    Animation::Map map;
    map[Animation::Stand] = {rec1, rec2, rec3, rec4, rec5};
    return map;
}
