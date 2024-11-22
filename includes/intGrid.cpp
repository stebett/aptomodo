//
// Created by ginko on 11/21/24.
//

#include "intGrid.h"

#include <constants.h>


[[nodiscard]] size_t clamp(const size_t value, const size_t min, const size_t max) {
    return std::max(min, std::min(value, max));
}

size_t IntGrid::worldToGrid(const float coord, const size_t maxIndex) {
    const int index = static_cast<int>(std::floor(coord / tileSize));
    return clamp(index, 0, maxIndex);
}

float gridToWorld(const int coord) {
    return static_cast<float>(coord * tileSize);
}

IntGrid::IntGrid() {
    for (auto &row: grid) {
        row.fill(IntValue::INVALID);
    }
}

const IntValue &IntGrid::operator()(const size_t row, const size_t col) const {
    if (row >= Rows || col >= Cols) {
        throw std::out_of_range("Index out of bounds");
    }
    return grid[row][col];
}

[[nodiscard]] size_t IntGrid::rows() { return Rows; }
[[nodiscard]] size_t IntGrid::cols() { return Cols; }

[[nodiscard]] const IntValue &IntGrid::safe(const size_t row, const size_t col) const {
    return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)];
}


[[nodiscard]] const IntValue &IntGrid::fromWorld(const float row, const float col) const {
    return grid[worldToGrid(row, Rows - 1)][worldToGrid(col, Cols - 1)];
}

[[nodiscard]] const IntValue &IntGrid::fromWorld(const Vector2 position) const {
    return grid[worldToGrid(position.x, Rows - 1)][worldToGrid(position.y, Cols - 1)];
}

[[nodiscard]] bool IntGrid::inWorldBounds(const float x, const float y) {
    return (x > 0) && (y > 0) && (x < Rows * tileSize) && (y < Cols * tileSize);
}

[[nodiscard]] bool IntGrid::isOpaque(const size_t row, const size_t col) const {
    return grid[clamp(row, 0, Rows - 1)][clamp(col, 0, Cols - 1)] == IntValue::OBSTACLE;
}

void IntGrid::setVisible(const size_t row, const size_t col) {
    DrawRectangle(row*tileSize, col*tileSize, tileSize, tileSize, ColorAlpha(YELLOW, 0.1));
    // grid[row][col] = IntValue::VISIBLE;
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
    for (int x = 0; x <= IntGrid::rows(); x++) {
        for (int y = 0; y <= IntGrid::cols(); y++) {
            switch (layer.getIntGridVal(x, y).value) {
                case -1: {
                    grid[x][y] = IntValue::EMPTY;
                    break;
                }
                case 1: {
                    grid[x][y] = IntValue::OBSTACLE;
                    break;
                }
                case 2: {
                    grid[x][y] = IntValue::NEAR_OBSTACLE;
                    break;
                }
                default:
                    grid[x][y] = IntValue::INVALID;
            }
        }
    }
}
