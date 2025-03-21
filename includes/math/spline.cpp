//
// Created by ginko on 11/30/24.
//

#include <math/spline.h>

EasingSpline::EasingSpline(float x1, float y1, float x2, float y2) : bezier({
                                                                                    {0.0f, 0.0f},
                                                                                    {x1,   y1},
                                                                                    {x2,   y2},
                                                                                    {1.0f, 1.0f}
                                                                            }) {
}

EasingSpline::EasingSpline(std::array<float, 4> points) : bezier({
                                                                         {0.0f,      0.0f},
                                                                         {points[0], points[1]},
                                                                         {points[2], points[3]},
                                                                         {1.0f,      1.0f}
                                                                 }) {
}

float EasingSpline::valueAt(float t) const {
    return bezier.valueAt(t).y;
}

EasingSpline::EasingSpline() : bezier({
                                              {0.0f, 0.0f},
                                              {0.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {1.0f, 1.0f}
                                      }) {

}

void EasingSpline::update(std::array<float, 4> points) {
    bezier = Bezier::Bezier<3>({{0.0f,      0.0f},
                                {points[0], points[1]},
                                {points[2], points[3]},
                                {1.0f,      1.0f}});
}

std::array<float, 4> EasingSpline::get() const {
    return {
            static_cast<float>(bezier[1].x),
            static_cast<float>(bezier[1].y),
            static_cast<float>(bezier[2].x),
            static_cast<float>(bezier[2].y)
    };
}

Spline::Spline(std::array<Math::Vec2, 4> points) : bezier({
                                                                  points[0],
                                                                  points[1],
                                                                  points[2],
                                                                  points[3],
                                                          }) {
}

float Spline::valueAt(float t) const {
    return bezier.valueAt(t).y;
}

std::array<Math::Vec2, 4> Spline::get() const {
    return {
            bezier[0],
            bezier[1],
            bezier[2],
            bezier[3],
    };
}
Spline::Spline() = default;
