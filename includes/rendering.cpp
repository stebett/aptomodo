//
// Created by ginko on 02/03/24.
//

#include "rendering.h"
#include "constants.h"
#include "factories.h"
#include <raymath.h>
#include "managers/animationManager.h"
#include "config.h"

void updateCamera(Camera2D &camera, Position &playerPosition) {
    float target_x = playerPosition.x;
    float target_y = playerPosition.y;

    // Don't show map end
    if (playerPosition.x < screenWidth / (2 * camera.zoom)) {
        target_x = screenWidth / (2 * camera.zoom);
    }
    if (playerPosition.x > mapWidth - screenWidth / (2 * camera.zoom)) {
        target_x = mapWidth - screenWidth / (2 * camera.zoom);
    }
    if (playerPosition.y < screenHeight / (2 * camera.zoom)) {
        target_y = screenHeight / (2 * camera.zoom);
    }
    if (playerPosition.y > mapHeight - screenHeight / (2 * camera.zoom)) {
        target_y = mapHeight - screenHeight / (2 * camera.zoom);
    }

    camera.target = Vector2Lerp(camera.target, {target_x, target_y}, 0.1);
}

void switchCamera(Camera2D &camera, Position &playerPosition) {
    camera.target = playerPosition;
}

//
//
//void drawAttacks(entt::registry &registry) {
//    auto effectView = registry.view<AttackEffect>();
//    for (auto [entity, effect]: effectView.each()) {
//        effect.Draw();
//        if (effect.Expired()) {
//            registry.destroy(entity);
//        }
//    }
//}
//
//void drawLivingBB(const entt::registry &registry) {
//    auto livingView = registry.view<Living, Radius, Position, ColorBB>();
//    for (auto [entity, radius, position, color]: livingView.each()) {
//        DrawCircle(position.x, position.y, radius, ColorAlpha(color, 0.3));
//    }
//}
//
//void drawEnemyTexture(const entt::registry &registry, unsigned int frame) {
//    auto livingView = registry.view<Living, Radius, Position, LookAngle, Speed>();
//    for (auto [entity, radius, position, rotation, speed]: livingView.each()) {
//        DrawTexturePro(AnimationManager::Instance().getTexture("enemy/walking/v1/", frame * speed.actual),
//                       {0, 0, 240, 240},
//                       {position.x, position.y, 60, 60},
//                       {30, 30},
//                       rotation + 90,
//                       WHITE);
//    }
//}
//
//void draw(entt::registry &registry, GameScene *scene, unsigned int frame) {
//    scene->draw();
//    drawLivingBB(registry);
//    drawEnemyTexture(registry, frame / config::enemy_walking_animation_fps);
//    drawAttacks(registry);
//}
//
//GameScene::GameScene(entt::registry &registry) : m_registry(registry) {
//    ldtkProject = new ldtk::Project();
//    std::cout << getAssetPath("project.ldtk") << "\n";
//    ldtkProject->loadFromFile(getAssetPath("project.ldtk"));
//
//    ldtkWorld = &ldtkProject->getWorld();
//
//    current_level = -1;
//    setLevel(registry, 0);
//}
//
//
//GameScene::~GameScene() {
//    delete ldtkProject;
//    delete ldtkWorld;
//
//    ldtkProject = nullptr;
//
//    UnloadTexture(renderedLevelTexture);
//    UnloadTexture(currentTilesetTexture);
////    m_registry.remove<Texture>();
//}
//
//void GameScene::draw() {
//    ClearBackground(BLUE);
//    // TODO update so that it draws only stuff onscreen, probably this should be in rendering Manager
//    DrawTextureRec(renderedLevelTexture,
//                   {0, 0, (float) renderedLevelTexture.width, (float) -renderedLevelTexture.height},
//                   {0, 0}, WHITE);
//
//}
//
//void GameScene::setLevel(entt::registry &registry, int level) {
//    // unload current tileset texture if necessary
//    if (current_level >= 0) {
//        UnloadTexture(currentTilesetTexture);
//    }
//
//    currentLdtkLevel = &ldtkWorld->getLevel(level);
//
//    auto levelSize = currentLdtkLevel->size;
//    auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);
//
//    BeginTextureMode(renderTexture);
//
////    if (currentLdtkLevel->hasBgImage()) {
////        auto backgroundPath = currentLdtkLevel->getBgImage();
////        auto backgroundTexture = LoadTexture(getAssetPath(backgroundPath.path.c_str()).c_str());
////        SetTextureFilter(backgroundTexture, TEXTURE_FILTER_POINT);
////
////        // tile background texture to cover the whole frame buffer
////        for (int i = 0; i <= (mapWidth / backgroundTexture.width); i++) {
////            for (int j = 0; j <= (mapHeight / backgroundTexture.height); j++) {
////                DrawTextureV(backgroundTexture,
////                             {float(i * backgroundTexture.width), float(j * backgroundTexture.height)}, WHITE);
////            }
////        }
////    }
//
//    // draw all tileset layers
//    for (auto &&layer: currentLdtkLevel->allLayers()) {
//        if (layer.hasTileset()) {
//            currentTilesetTexture = LoadTexture((getAssetPath(layer.getTileset().path)).c_str());
//            // if it is a tile layer then draw every tile to the frame buffer
//            for (auto &&tile: layer.allTiles()) {
//                auto source_pos = tile.getTextureRect();
//                auto tile_size = float(layer.getTileset().tile_size);
//
//                Rectangle source_rect = {
//                        .x = float(source_pos.x),
//                        .y = float(source_pos.y),
//                        .width = tile.flipX ? -tile_size : tile_size,
//                        .height = tile.flipY ? -tile_size : tile_size,
//                };
//
//                Vector2 target_pos = {
//                        (float) tile.getPosition().x,
//                        (float) tile.getPosition().y,
//                };
//
//                DrawTextureRec(currentTilesetTexture, source_rect, target_pos, WHITE);
//            }
//        }
//
//        if (ldtk::LayerType::IntGrid == layer.getType()) {
//            const auto gridSize = layer.getGridSize();
////            std::cout << gridSize.x << " " << gridSize.y << "\n";
////            std::cout << grid.rows() << " " << grid.cols() << "\n";
//            for (int x = 0; x <= mapWidth / tileSize; x++) {
//                for (int y = 0; y <= mapHeight / tileSize; y++) {
//                    grid(x, y) = layer.getIntGridVal(x, y).value;
//                }
//            }
//        }
//    }
//
//    for (auto &&entity: currentLdtkLevel->getLayer("Entities").allEntities()) {
//        Position position = {static_cast<float>(entity.getPosition().x), static_cast<float>(entity.getPosition().y)};
//        if (entity.getName() == "Player") {
//            spawnPlayer(registry, position, this);
//        }
//
//        if (entity.getName() == "Enemy") {
//            spawnEnemy(registry, position);
//        }
//    }
//
//
//    EndTextureMode();
//    renderedLevelTexture = renderTexture.texture;
//
//}
//
////std::string GameScene::getTexturePath(const std::string &tileset) {
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
