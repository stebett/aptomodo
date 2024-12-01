//
// Created by ginko on 11/30/24.
//

#ifndef SPLINE_H
#define SPLINE_H
#include "bezier.h"
#include "math/mathConstants.h"


// p1, c1, c2, p2
struct LocalSpline {
    std::array<b2Vec2, 4> localPoints{};
    std::array<b2Transform, 4> transforms{};
    b2Rot rotation{};

    LocalSpline(std::array<Vector2, 4> points, b2Rot rotation);
    LocalSpline(const std::array<b2Vec2, 4> &points, std::array<b2Transform, 4> transforms);

    [[nodiscard]] std::array<Vector2, 4> getGlobal(b2Transform);
    [[nodiscard]] Bezier::Bezier<3> getLocalBezier() const;
    [[nodiscard]] std::array<Math::Vec2, 4> getLocal() const;
};

struct EasingSpline {
    Bezier::Bezier<3> bezier;
    EasingSpline(float x1, float y1, float x2, float y2);
    float valueAt(float t) const;
};

const EasingSpline LinearEasing {0, 0, 1, 1};



#endif //SPLINE_H
