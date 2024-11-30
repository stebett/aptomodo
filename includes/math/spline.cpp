//
// Created by ginko on 11/30/24.
//

#include "spline.h"

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

std::array<Math::Vec2, 4> LocalSpline::getLocal() const {
    return {
        b2Sub(localPoints[0], transforms[0].p),
        b2Sub(localPoints[1], transforms[1].p),
        b2Sub(localPoints[2], transforms[2].p),
        b2Sub(localPoints[3], transforms[3].p)
    };
}
