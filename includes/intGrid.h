//
// Created by ginko on 11/21/24.
//

#ifndef INTGRID_H
#define INTGRID_H
#include <constants.h>


enum class IntValue {
    EMPTY = -1,
    INVALID = 0,
    OBSTACLE = 1,
    NEAR_OBSTACLE = 2,
    NPC = 3,
    VISIBLE = 10,
};

using Map = std::array<std::array<IntValue, mapHeight / tileSize + 1>, mapWidth / tileSize + 1>;

class IntGrid {
private:
    Map grid{};
    static constexpr size_t Rows{Map().max_size()};
    static constexpr size_t Cols{Map()[0].max_size()};

public:
    IntGrid();

    void initialize(const ldtk::Layer &layer);

    [[nodiscard]] const IntValue &operator()(size_t row, size_t col) const;

    [[nodiscard]] static size_t rows();

    [[nodiscard]] static size_t cols();

    [[nodiscard]] const IntValue &safe(size_t row, size_t col) const;

    [[nodiscard]] const IntValue &fromWorld(float row, float col) const;

    [[nodiscard]] const IntValue &fromWorld(Vector2 position) const;

    [[nodiscard]] static bool inWorldBounds(float x, float y);

    [[nodiscard]] bool isOpaque(size_t row, size_t col) const;

    void setVisible(size_t row, size_t col);

};

#endif //INTGRID_H
