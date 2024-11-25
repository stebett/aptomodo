//
// Created by ginko on 11/21/24.
//

#ifndef INTGRID_H
#define INTGRID_H
#include <constants.h>

// Todo this file can probably be in a different directory


enum class IntValue {
    EMPTY = -1,
    INVALID = 0,
    OBSTACLE = 1,
    NEAR_OBSTACLE = 2,
    NPC = 3,
    VISIBLE = 10,
};


// TODO this needs to be an array of a bitfield, so that we can set the tiles to multiple values at the same time
using Map = std::array<std::array<IntValue, IntGridHeight>, IntGridWidth>;

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

    static size_t worldToGrid(float coord, size_t maxIndex);

    void setVisible(size_t row, size_t col);

};

#endif //INTGRID_H
