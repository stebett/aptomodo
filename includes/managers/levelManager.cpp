//
// Created by ginko on 17/04/24.
//

#include "pch.h"
#include "levelManager.h"
#include <components.h>

std::vector<std::pair<std::string, Vector2> > LevelManager::entitiesPositions;
LevelManager *LevelManager::instance;
int LevelManager::current_level;
ldtk::Project *LevelManager::ldtkProject;
const ldtk::World *LevelManager::ldtkWorld;
const ldtk::Level *LevelManager::currentLdtkLevel;
IntGrid<mapWidth / tileSize + 1, mapHeight / tileSize + 1> LevelManager::grid;
Texture2D LevelManager::currentTilesetTexture;
Texture2D LevelManager::renderedLevelTexture;

void LevelManager::Instantiate() {
    instance = new LevelManager();

    ldtkProject = new ldtk::Project();
    std::cout << getAssetPath("project.ldtk") << "\n";
    ldtkProject->loadFromFile(getAssetPath("project.ldtk"));

    ldtkWorld = &ldtkProject->getWorld();

    current_level = -1;
    SetLevel(0);
}


LevelManager &LevelManager::Instance() {
    return *instance;
}


LevelManager::~LevelManager() {
    delete instance;
    delete ldtkProject;
    delete ldtkWorld;

    ldtkProject = nullptr;

    UnloadTexture(renderedLevelTexture);
    UnloadTexture(currentTilesetTexture);
}

void LevelManager::SetLevel(int level) {
    // unload current tileset texture if necessary
    if (current_level >= 0) {
        UnloadTexture(currentTilesetTexture);
    }

    currentLdtkLevel = &ldtkWorld->getLevel(level);

    auto levelSize = currentLdtkLevel->size;
    auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);

    BeginTextureMode(renderTexture);

    //    if (currentLdtkLevel->hasBgImage()) {
    //        auto backgroundPath = currentLdtkLevel->getBgImage();
    //        auto backgroundTexture = LoadTexture(getAssetPath(backgroundPath.path.c_str()).c_str());
    //        SetTextureFilter(backgroundTexture, TEXTURE_FILTER_POINT);
    //
    //        // tile background texture to cover the whole frame buffer
    //        for (int i = 0; i <= (mapWidth / backgroundTexture.width); i++) {
    //            for (int j = 0; j <= (mapHeight / backgroundTexture.height); j++) {
    //                DrawTextureV(backgroundTexture,
    //                             {float(i * backgroundTexture.width), float(j * backgroundTexture.height)}, WHITE);
    //            }
    //        }
    //    }

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
                // DrawText(std::format("{}", target_pos.y).c_str(), target_pos.x, target_pos.y, 2, BLACK);
            }
        }

        if (ldtk::LayerType::IntGrid == layer.getType()) {
            const auto gridSize = layer.getGridSize();
            //            std::cout << gridSize.x << " " << gridSize.y << "\n";
            //            std::cout << grid.rows() << " " << grid.cols() << "\n";
            for (int x = 0; x <= mapWidth / tileSize; x++) {
                for (int y = 0; y <= mapHeight / tileSize; y++) {
                    switch (layer.getIntGridVal(x, y).value) {
                        case -1: {
                            grid(x, y) = IntValue::EMPTY;
                            break;
                        }
                        case 1: {
                            grid(x, y) = IntValue::OBSTACLE;
                            break;
                        }
                        case 2: {
                            grid(x, y) = IntValue::NEAR_OBSTACLE;
                            break;
                        }
                        default:
                            grid(x, y) = IntValue::INVALID;
                    }
                }
            }
        }
    }

    for (auto &&entity: currentLdtkLevel->getLayer("Entities").allEntities()) {
        Vector2 position = {static_cast<float>(entity.getPosition().x), static_cast<float>(entity.getPosition().y)};
        entitiesPositions.emplace_back(entity.getName(), position);
    }


    EndTextureMode();
    renderedLevelTexture = renderTexture.texture;
    // SetTextureWrap(renderedLevelTexture, TEXTURE_WRAP_MIRROR_CLAMP);
}

std::vector<std::pair<std::string, Vector2> > &LevelManager::GetEntitiesPositions() {
    return entitiesPositions;
}

void LevelManager::Draw(Camera2D &camera) {
    // TODO update so that it draws only stuff onscreen, probably this should be in rendering Manager
    DrawTextureRec(renderedLevelTexture,
                   {0, 0, (float) renderedLevelTexture.width, (float) -renderedLevelTexture.height},
                   {0, 0}, WHITE);
}

void LevelManager::Update(const entt::registry &registry) {
    auto enemyView = registry.view<Living, Radius, Position, Enemy>();
    for (int row = 0; row < LevelManager::grid.rows(); row++) {
        for (int col = 0; col < LevelManager::grid.cols(); col++) {
            if (grid(row, col) == IntValue::NPC) grid(row, col) = IntValue::EMPTY;

            Rectangle rec = {float(tileSize * row), float(tileSize * col), tileSize, tileSize};
            for (auto [enemy, radius, position]: enemyView.each()) {
                // if (grid(row, col) == -1 && CheckCollisionCircleRec(position, radius, rec))
                // LevelManager::grid(row, col) = 3;
                LevelManager::grid.fromWorld(position) = IntValue::NPC;
            }
        }
    }
}


//std::string GameScene::getTexturePath(const std::string &tileset) {
//    return getAssetPath(ldtkWorld->getTileset(tileset).path);
//}

//Animation::Map GameScene::getAnimationMap(const std::string &ldtkEnum) {
//    auto &playerEnum = ldtkWorld->getEnum(ldtkEnum);
//    auto rec1 = toRectangle(playerEnum["Standing1"].getIconTextureRect());
//    auto rec2 = toRectangle(playerEnum["Standing2"].getIconTextureRect());
//    auto rec3 = toRectangle(playerEnum["Standing3"].getIconTextureRect());
//    auto rec4 = toRectangle(playerEnum["Standing4"].getIconTextureRect());
//    auto rec5 = toRectangle(playerEnum["Standing5"].getIconTextureRect());
//    Animation::Map map;
//    map[Animation::Stand] = {rec1, rec2, rec3, rec4, rec5};
//    return map;
//}
