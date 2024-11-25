//
// Created by ginko on 17/04/24.
//

#include "levelManager.h"
#include "intGrid.h"

std::vector<std::pair<std::string, Vector2> > LevelManager::entitiesPositions;
std::vector<std::pair<std::string, Vector2> > LevelManager::enemiesPositions;
LevelManager *LevelManager::instance;
int LevelManager::current_level;
ldtk::Project *LevelManager::ldtkProject;
const ldtk::World *LevelManager::ldtkWorld;
const ldtk::Level *LevelManager::currentLdtkLevel;
IntGrid LevelManager::grid;
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

void LevelManager::SetLevel(const int level) {
    // unload current tileset texture if necessary
    if (current_level >= 0) {
        UnloadTexture(currentTilesetTexture);// todo unload after drawing?
    }

    currentLdtkLevel = &ldtkWorld->getLevel(level);

    auto levelSize = currentLdtkLevel->size;
    auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);

    BeginTextureMode(renderTexture);

    for (auto &&layer: currentLdtkLevel->allLayers()) {
        if (layer.hasTileset()) {
            currentTilesetTexture = LoadTexture((getAssetPath(layer.getTileset().path)).c_str());
            // if it is a tile layer then draw every tile to the frame buffer
            for (auto &&tile: layer.allTiles()) {
                const auto source_pos = tile.getTextureRect();
                const auto tile_size = static_cast<float>(layer.getTileset().tile_size);
                const Rectangle source_rect = {
                    .x = static_cast<float>(source_pos.x),
                    .y = static_cast<float>(source_pos.y),
                    .width = tile.flipX ? -tile_size : tile_size,
                    .height = tile.flipY ? -tile_size : tile_size,
                };

                const Vector2 target_pos = {
                    static_cast<float>(tile.getPosition().x),
                    static_cast<float>(tile.getPosition().y),
                };

                DrawTextureRec(currentTilesetTexture, source_rect, target_pos, WHITE);
            }
        }
        if (ldtk::LayerType::IntGrid == layer.getType()) { grid.initialize(layer); }
    }

    for (const auto &entity: currentLdtkLevel->getLayer("Entities").allEntities()) {
        Vector2 position = {static_cast<float>(entity.getPosition().x), static_cast<float>(entity.getPosition().y)};
        if (entity.getName() == "Enemy") {
            auto enemyName = entity.getField<ldtk::FieldType::String>("EnemyName").value();
            enemiesPositions.emplace_back(enemyName, position);
        }
        entitiesPositions.emplace_back(entity.getName(), position); // TODO will need to specialize this so different enemies get their own position
        std::cout << entity.getName() << ", {" << position.x << ", " << position.y << "}\n";
    }



    EndTextureMode();
    renderedLevelTexture = renderTexture.texture;
    // SetTextureWrap(renderedLevelTexture, TEXTURE_WRAP_MIRROR_CLAMP); // TODO force zoom to stay in map boundaries instead?
}

std::vector<std::pair<std::string, Vector2> > &LevelManager::GetEntitiesPositions() {
    return entitiesPositions;
}

std::vector<std::pair<std::string, Vector2> > &LevelManager::GetEnemiesPositions() {
    return enemiesPositions;
}


void LevelManager::Update(const entt::registry &registry) {
    // auto enemyView = registry.view<Living, Radius, Position, Enemy>();
    // for (int row = 0; row < LevelManager::grid.rows(); row++) {
    //     for (int col = 0; col < LevelManager::grid.cols(); col++) {
    //         if (grid(row, col) == IntValue::NPC) grid(row, col) = IntValue::EMPTY;
    //
    //         Rectangle rec = {float(tileSize * row), float(tileSize * col), tileSize, tileSize};
    //         for (auto [enemy, radius, position]: enemyView.each()) {
    //             // if (grid(row, col) == -1 && CheckCollisionCircleRec(position, radius, rec))
    //             // LevelManager::grid(row, col) = 3;
    //             // if (grid.fromWorld(position) == IntValue::EMPTY) grid.fromWorld(position) = IntValue::NPC;
    //         }
    //     }
    // }
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
