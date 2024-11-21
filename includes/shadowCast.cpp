//
// Created by ginko on 11/21/24.
// From https://www.roguebasin.com/index.php/C++_shadowcasting_implementation
// A C++ implementation of Bjorn Bergstrom's recursive shadowcasting FOV algorithm.

#include "shadowCast.h"


void shadowCast(IntGrid &map, uint startX, uint startY, uint radius, const int row,
                float startSlope, const float endSlope, uint xx, uint xy, uint yx, uint yy) {
    if (startSlope < endSlope) return;
    float nextStartSlope = startSlope;
    for (int i = row; i <= radius; i++) {
        bool blocked = false;
        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            const float leftSlope = (dx - 0.5f) / (dy + 0.5f);
            const float rightSlope = (dx + 0.5f) / (dy - 0.5f);
            if (startSlope < rightSlope) continue;
            if (endSlope > leftSlope) break;

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;
            if ((sax < 0 && static_cast<uint>(std::abs(sax)) > startX) ||
                (say < 0 && static_cast<uint>(std::abs(say)) > startY)) {
                continue;
            }
            uint ax = startX + sax;
            uint ay = startY + say;
            if (ax >= map.rows() || ay >= map.cols()) continue;

            const uint radius2 = radius * radius;
            if ((uint) (dx * dx + dy * dy) < radius2) {
                map.setVisible(ax, ay); // Should check here if player is visible
            }

            if (blocked) {
                if (map.isOpaque(ax, ay)) {
                    nextStartSlope = rightSlope;
                    continue;
                }
                blocked = false;
                startSlope = nextStartSlope;
            } else if (map.isOpaque(ax, ay)) {
                blocked = true;
                nextStartSlope = rightSlope;
                shadowCast(map, startX, startY, radius, i + 1, startSlope, leftSlope, xx, xy, yx, yy);
            }
        }
        if (blocked) {
            break;
        }
    }
}

static constexpr int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

void do_fov(IntGrid &map, const uint startX, const uint startY, const uint radius) {
    for (uint i = 0; i < 8; i++) {
        shadowCast(map, startX, startY, radius, 1, 1.0, 0.0,
                   multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
    }
}
