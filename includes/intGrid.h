//
// Created by ginko on 11/21/24.
//

#ifndef INTGRID_H
#define INTGRID_H
#include <constants.h>

// Todo this file can probably be in a different directory


namespace IntValue {
    enum {
        EMPTY = 0,
        OBSTACLE,
        NEAR_OBSTACLE,
        OPAQUE,
        NPC,
        VISIBLE,
        GRASS,
        WATER,
        SAND,
    };
};

constexpr int intGridValues = 32;
using GridBitmap = std::bitset<intGridValues>;
using Map = std::array<std::array<GridBitmap, Const::IntGridHeight>, Const::IntGridWidth>;

class IntGrid {
private:
    Map grid{};
    static constexpr size_t Rows{Map().max_size()};
    static constexpr size_t Cols{Map()[0].max_size()};

public:
    IntGrid();

    void initialize(const ldtk::Layer &layer);

    [[nodiscard]] const GridBitmap &operator()(size_t row, size_t col) const;

    [[nodiscard]] static size_t rows();

    [[nodiscard]] static size_t cols();

    [[nodiscard]] const GridBitmap &safe(size_t row, size_t col) const;

    [[nodiscard]] const GridBitmap &fromWorld(float row, float col) const;

    [[nodiscard]] const GridBitmap &fromWorld(Vector2 position) const;

    [[nodiscard]] static bool inWorldBounds(float x, float y);

    [[nodiscard]] bool isOpaque(size_t row, size_t col) const;

    static size_t worldToGrid(float coord, size_t maxIndex);

    void setVisible(size_t row, size_t col);
};

#endif //INTGRID_H
