//
// Created by ginko on 11/30/24.
//

#include <math/spline.h>

LocalSpline::LocalSpline(const std::array<b2Vec2, 4> &points,
                         std::array<b2Transform, 4> transforms) : localPoints(points), transforms(transforms) {
    for (auto &t: transforms) { t.q = b2Rot(cos(0), sin(0)); }
}


std::array<Vector2, 4> LocalSpline::getGlobal(const b2Transform ref) {
    // return {
    //     Math::Vec2(b2Add(b2Sub(localPoints[0], transforms[0].p), ref.p)),
    //     Math::Vec2(b2Add(b2Sub(localPoints[1], transforms[1].p), ref.p)),
    //     Math::Vec2(b2Add(b2Sub(localPoints[2], transforms[2].p), ref.p)),
    //     Math::Vec2(b2Add(b2Sub(localPoints[3], transforms[3].p), ref.p)),
    // };

    return {
            Math::Vec2(b2TransformPoint(ref, b2InvTransformPoint(transforms[0], localPoints[0]))),
            Math::Vec2(b2TransformPoint(ref, b2InvTransformPoint(transforms[1], localPoints[1]))),
            Math::Vec2(b2TransformPoint(ref, b2InvTransformPoint(transforms[2], localPoints[2]))),
            Math::Vec2(b2TransformPoint(ref, b2InvTransformPoint(transforms[3], localPoints[3]))),
    };
}

Bezier::Bezier<3> LocalSpline::getLocalBezier() const {
    auto points = getLocal();
    return Bezier::Bezier<3>({
                                     {points[0].x, points[0].y},
                                     {points[1].x, points[1].y},
                                     {points[2].x, points[2].y},
                                     {points[3].x, points[3].y},
                             });
}

std::array<Math::Vec2, 4> LocalSpline::getLocal() const {
    return {
            b2Sub(localPoints[0], transforms[0].p),
            b2Sub(localPoints[1], transforms[1].p),
            b2Sub(localPoints[2], transforms[2].p),
            b2Sub(localPoints[3], transforms[3].p)
    };
}

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

std::array<float, 4> EasingSpline::get() const{
    return {
            static_cast<float>(bezier[1].x),
            static_cast<float>(bezier[1].y),
            static_cast<float>(bezier[2].x),
            static_cast<float>(bezier[2].y)
    };
}
