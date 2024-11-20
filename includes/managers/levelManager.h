//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_LEVELMANAGER_H
#define APTOMODO_LEVELMANAGER_H

#include <raylib.h>
#include <string>
#include <entt/entity/registry.hpp>
#include <LDtkLoader/Project.hpp>
#include "../constants.h"

enum class IntValue {
    EMPTY = -1,
    INVALID = 0,
    OBSTACLE = 1,
    NEAR_OBSTACLE = 2,
    NPC = 3,
};


template<size_t Rows, size_t Cols>
class IntGrid {
private:
    std::array<std::array<IntValue, Cols>, Rows> grid;

    [[nodiscard]] static size_t clamp(const size_t value, const size_t min, const size_t max) {
        return std::max(min, std::min(value, max));
    }

    static size_t worldToGrid(const float coord, const size_t maxIndex) {
        const int index = static_cast<int>(std::floor(coord / tileSize));
        return clamp(index, 0, maxIndex);
    }

    static float gridToWorld(const int coord) {
        return static_cast<float>(coord * tileSize);
    }

public:
    IntGrid() {
        for (auto &row: grid) {
            row.fill(IntValue::INVALID);
        }
    }

    IntValue &operator()(size_t row, size_t col) {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return grid[row][col];
    }

    const IntValue &operator()(size_t row, size_t col) const {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return grid[row][col];
    }

    [[nodiscard]] static size_t rows() { return Rows; }
    [[nodiscard]] static size_t cols() { return Cols; }

    IntValue &safe(const size_t row, const size_t col) {
        return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)];
    }

    [[nodiscard]] const IntValue &safe(const size_t row, const size_t col) const {
        return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)];
    }

    IntValue &fromWorld(const float row, const float col) {
        return grid[worldToGrid(row, Rows - 1)][worldToGrid(col, Cols - 1)];
    }

    [[nodiscard]] const IntValue &fromWorld(const float row, const float col) const {
        return grid[worldToGrid(row, Rows - 1)][worldToGrid(col, Cols - 1)];
    }

    IntValue &fromWorld(const Vector2 position) {
        return grid[worldToGrid(position.x, Rows - 1)][worldToGrid(position.y, Cols - 1)];
    }

    [[nodiscard]] const IntValue &fromWorld(const Vector2 position) const {
        return grid[worldToGrid(position.x, Rows - 1)][worldToGrid(position.y, Cols - 1)];
    }

    [[nodiscard]] static bool inWorldBounds(const float x, const float y) {
        return (x > 0) && (y > 0) && (x < Rows*tileSize) && (y < Cols*tileSize);
    }


    void display() const {
        for (const auto &row: grid) {
            for (const auto &val: row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
};


class LevelManager {
    static const char *root;
    static LevelManager *instance;

    static int current_level;

    static ldtk::Project *ldtkProject;
    static const ldtk::World *ldtkWorld;
    static const ldtk::Level *currentLdtkLevel;

    static Texture2D currentTilesetTexture;

    static std::vector<std::pair<std::string, Vector2> > entitiesPositions;
    static std::hash<std::string> hasher;

public:
    static IntGrid<mapWidth / tileSize + 1, mapHeight / tileSize + 1> grid;
    static Texture2D renderedLevelTexture;


    ~LevelManager();

    // TODO add an update function which keeps tracks of all living positions and ranges and updates the grid based on it

    static std::vector<std::pair<std::string, Vector2> > &GetEntitiesPositions();

    static void Update(const entt::registry &registry);

    static void SetLevel(int level);

    static LevelManager &Instance();

    static void Instantiate();
};


#endif //APTOMODO_LEVELMANAGER_H
