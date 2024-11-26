//
// Created by ginko on 17/04/24.
//

#include "levelManager.h"
#include "intGrid.h"

namespace Level {
    std::vector<Level::Entity> LoadEntities(const ldtk::Level &level) {
        std::vector<Level::Entity> result = {};
        for (const auto &entity: level.getLayer("Entities").allEntities()) {
            const Vector2 position = {
                static_cast<float>(entity.getPosition().x), static_cast<float>(entity.getPosition().y)
            };
            const auto &type = entity.getName();
            const auto name = entity.getField<ldtk::FieldType::String>("Name").value();
            result.emplace_back(position, type, name);
        }
        return result;
    }

    Texture2D LoadLevelTexture(const ldtk::Level &level) {
        const auto levelSize = level.size;
        const auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);
        BeginTextureMode(renderTexture);

        for (auto &&layer: level.allLayers()) {
            if (layer.hasTileset()) {
                const Texture2D tilesetTexture = LoadTexture(getAssetPath(layer.getTileset().path).c_str());
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

                    DrawTextureRec(tilesetTexture, source_rect, target_pos, WHITE);
                }
                // UnloadTexture(tilesetTexture);
            }
        }
        EndTextureMode();
        return renderTexture.texture;
    }

    IntGrid LoadIntGrid(const ldtk::Level &level) {
        IntGrid grid;
        for (auto &layer: level.allLayers())
            if (ldtk::LayerType::IntGrid == layer.getType())
                grid.initialize(layer);
        return grid;
    }
};