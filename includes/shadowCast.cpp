//
// Created by ginko on 11/21/24.
// From https://www.roguebasin.com/index.php/C++_shadowcasting_implementation
// A C++ implementation of Bjorn Bergstrom's recursive shadowcasting FOV algorithm.

#include "shadowCast.h"

float calculateAngle(int dx, int dy) {
    return std::atan2(dy, dx) * 180 / M_PI;
}


// Normalize an angle to [0, 360)
float normalizeAngle(float angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

// Check if `testAngle` is touched when moving `startAngle` to `endAngle` clockwise
bool isAngleTouched(float startAngle, float endAngle, float testAngle) {
    // Normalize all angles to [0, 360)
    startAngle = normalizeAngle(startAngle);
    endAngle = normalizeAngle(endAngle);
    testAngle = normalizeAngle(testAngle);

    // Case 1: Arc does not cross 0 degrees
    if (startAngle >= endAngle) {
        return testAngle <= startAngle && testAngle >= endAngle;
    }
    // Case 2: Arc crosses 0 degrees
    else {
        return testAngle <= startAngle || testAngle >= endAngle;
    }
}


void computeFOV2Visualize(IntGrid &map, uint startX, uint startY, const uint radius, const int row,
                float startSlope, const float endSlope, uint xx, uint xy, uint yx, uint yy,
                float sectorStartAngle, float sectorEndAngle) {
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
            float angle = calculateAngle(sax, say);
            if (isAngleTouched(sectorStartAngle, sectorEndAngle, angle)) continue;

            uint ax = startX + sax;
            uint ay = startY + say;
            if (ax >= map.rows() || ay >= map.cols()) continue;

            const uint radius2 = radius * radius;
            if ((uint)(dx * dx + dy * dy) < radius2) {

                map.setVisible(ax, ay);
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
                computeFOV2Visualize(map, startX, startY, radius, i + 1, startSlope, leftSlope, xx, xy, yx, yy,
                           sectorStartAngle, sectorEndAngle);
            }
        }
        if (blocked) {
            break;
        }
    }
}

bool computeFOV(IntGrid &map, uint startX, uint startY, uint targetX, uint targetY, const uint radius, const int row,
                float startSlope, const float endSlope, uint xx, uint xy, uint yx, uint yy,
                float sectorStartAngle, float sectorEndAngle) {
    if (startSlope < endSlope) return false;
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
            float angle = calculateAngle(sax, say);
            if (isAngleTouched(sectorStartAngle, sectorEndAngle, angle)) continue;

            uint ax = startX + sax;
            uint ay = startY + say;
            if (ax >= map.rows() || ay >= map.cols()) continue;

            const uint radius2 = radius * radius;
            if ((uint)(dx * dx + dy * dy) < radius2) {
                if (ax == targetX && ay == targetY) return true;
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
                bool res = computeFOV(map, startX, startY, targetX, targetY, radius, i + 1, startSlope, leftSlope, xx, xy, yx, yy,
                           sectorStartAngle, sectorEndAngle);
                if (res) {return true;}
            }
        }
        if (blocked) {
            break;
        }
    }
    return false;
}



static constexpr int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

void visualizeFOV(IntGrid &map, const float startX, const float startY, const float radius,
            const float sectorStartAngle, const float sectorEndAngle) {

    const uint correctedX = IntGrid::worldToGrid(startX, IntGrid::rows());
    const uint correctedY = IntGrid::worldToGrid(startY, IntGrid::cols());
    const uint correctedRadius = IntGrid::worldToGrid(radius, IntGrid::rows());
    for (uint i = 0; i < 8; i++) {
        computeFOV2Visualize(map, correctedX, correctedY, correctedRadius, 1, 1.0, 0.0,
                   multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i],
                   sectorStartAngle, sectorEndAngle);
    }
}


bool isTargetInFOV(IntGrid &map, float startX, float startY, float targetX, float targetY,
    float radius,float sectorStartAngle, float sectorEndAngle) {
    const uint correctedX = IntGrid::worldToGrid(startX, IntGrid::rows());
    const uint correctedY = IntGrid::worldToGrid(startY, IntGrid::cols());
    const uint correctedTargetX = IntGrid::worldToGrid(targetX, IntGrid::rows());
    const uint correctedTargetY = IntGrid::worldToGrid(targetY, IntGrid::cols());
    const uint correctedRadius = IntGrid::worldToGrid(radius, IntGrid::rows());
    for (uint i = 0; i < 8; i++) {
        if (computeFOV(map, correctedX, correctedY, correctedTargetX,correctedTargetY, correctedRadius, 1, 1.0, 0.0,
                   multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i],
                   sectorStartAngle, sectorEndAngle))
            return true;
    }
    return false;
}