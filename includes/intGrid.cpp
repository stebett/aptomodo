//
// Created by ginko on 11/21/24.
//

#include "intGrid.h"

#include <constants.h>


[[nodiscard]] size_t clamp(const size_t value, const size_t min, const size_t max) {
    return std::max(min, std::min(value, max));
}

size_t IntGrid::worldToGrid(const float coord, const size_t maxIndex) {
    const int index = static_cast<int>(std::floor(coord / Const::tileSize));
    return clamp(index, 0, maxIndex);
}

float gridToWorld(const int coord) {
    return static_cast<float>(coord * Const::tileSize);
}

IntGrid::IntGrid() {
    for (auto &row: grid) {
        row.fill(IntValue::EMPTY);
    }
}

const GridBitmap &IntGrid::operator()(const size_t row, const size_t col) const {
    if (row >= Rows || col >= Cols) {
        throw std::out_of_range("Index out of bounds");
    }
    return grid[row][col];
}

[[nodiscard]] size_t IntGrid::rows() { return Rows; }
[[nodiscard]] size_t IntGrid::cols() { return Cols; }

[[nodiscard]] const GridBitmap &IntGrid::safe(const size_t row, const size_t col) const {
    return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)];
}


[[nodiscard]] const GridBitmap &IntGrid::fromWorld(const float row, const float col) const {
    return grid[worldToGrid(row, Rows - 1)][worldToGrid(col, Cols - 1)];
}

[[nodiscard]] const GridBitmap &IntGrid::fromWorld(const Vector2 position) const {
    return grid[worldToGrid(position.x, Rows - 1)][worldToGrid(position.y, Cols - 1)];
}

[[nodiscard]] bool IntGrid::inWorldBounds(const float x, const float y) {
    return (x > 0) && (y > 0) && (x < Rows * Const::tileSize) && (y < Cols * Const::tileSize);
}

[[nodiscard]] bool IntGrid::isOpaque(const size_t row, const size_t col) const {
    return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)][IntValue::OPAQUE];
}

void IntGrid::setVisible(const size_t row, const size_t col) {
    DrawRectangle(row*Const::tileSize, col*Const::tileSize, Const::tileSize, Const::tileSize, ColorAlpha(YELLOW, 0.1));
    // grid[row][col][IntValue::VISIBLE);
}

void IntGrid::setOccupied(const Vector2 &pos) {
    grid[pos.x][pos.y].set(IntValue::NPC);
}

void IntGrid::setFree(const Vector2 &pos) {
    grid[pos.x][pos.y][IntValue::NPC] = false;
}

// void IntGrid::reset() {
//     for (size_t row=0; row < rows(); ++row) {
//         for (size_t col=0; col < cols(); ++col) {
//             if (grid[row][col] == IntValue::VISIBLE) grid[row][col] = ?
//         }
//     }
// }


void IntGrid::initialize(const ldtk::Layer &layer) {
    // const auto gridSize = layer.getGridSize();
    for (int x = 0; x <= IntGrid::rows() -1; x++) {
        for (int y = 0; y <= IntGrid::cols()-1; y++) {
            switch (layer.getIntGridVal(x, y).value) {
                case 1: {
                    grid[x][y].set(IntValue::GRASS);
                    grid[x][y].set(IntValue::EMPTY);
                    break;
                }
                case 2: {
                    grid[x][y].set(IntValue::OBSTACLE);
                    grid[x][y].set(IntValue::WATER);
                    grid[x][y].set(IntValue::OPAQUE);
                    break;
                }
                case 3: {
                    grid[x][y].set(IntValue::NEAR_OBSTACLE);
                    grid[x][y].set(IntValue::SAND);
                    break;
                }
                default:
                    grid[x][y].set(IntValue::EMPTY);
            }
        }
    }
}
