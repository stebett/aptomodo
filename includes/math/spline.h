//
// Created by ginko on 11/30/24.
//

#ifndef SPLINE_H
#define SPLINE_H

#include "bezier.h"
#include "math/mathConstants.h"

struct Spline {
  Spline();
  Bezier::Bezier<3> bezier;
    // p1, c1, c2, p2
    explicit Spline(std::array<Math::Vec2, 4> points);

    [[nodiscard]] float valueAt(float t) const;
    [[nodiscard]] std::array<Math::Vec2, 4> get() const;

};

struct EasingSpline {
    Bezier::Bezier<3> bezier;

    EasingSpline(float x1, float y1, float x2, float y2);
    EasingSpline();

    explicit EasingSpline(std::array<float, 4> points);

    [[nodiscard]] float valueAt(float t) const;
    void update(std::array<float, 4>);
    [[nodiscard]] std::array<float, 4> get() const;
};

const EasingSpline LinearEasing{0, 0, 1, 1};


#endif //SPLINE_H
