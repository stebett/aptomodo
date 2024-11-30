//
// Created by ginko on 11/28/24.
//

#ifndef MATHCONSTANTS_H
#define MATHCONSTANTS_H

#include "math/bezier.h"

namespace Math {
    namespace Const {
        constexpr float radToDeg = (180.0 / 3.141592653589793238463);
        constexpr float degToRad = (3.141592653589793238463 / 180.0);
    }

    class Vec2 {
    public:
        float x;
        float y;

        Vec2() : x{0}, y{0} {
        };

        Vec2(Vector2 vec) : x{vec.x}, y{vec.y} {
        };

        Vec2(b2Vec2 vec) : x{vec.x}, y{vec.y} {
        };

        Vec2(Bezier::Point vec) : x{static_cast<float>(vec.x)}, y{static_cast<float>(vec.y)} {
        };


        operator Vector2() const noexcept {
            return {x, y};
        }

        operator b2Vec2() const noexcept {
            return {x, y};
        }

        operator Bezier::Point() const noexcept {
            return {x, y};
        }


    };
}
#endif //MATHCONSTANTS_H
