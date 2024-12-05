//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H

#include <math/spline.h>

#include "math/bezier.h"


namespace Attacks {
    struct Hit {
    };

    void Update();

    class Attack {
    public:
        float damage = 0;

        explicit Attack(float damage);;
    };

    class LocalTransform {
    public:
        b2Transform transformBegin;
        b2Transform transformEnd;

        explicit LocalTransform(b2Transform transform) : transformBegin(transform), transformEnd(transform) {
        };

        explicit LocalTransform(b2Transform t1, b2Transform t2) : transformBegin(t1), transformEnd(t2) {
        };

        [[nodiscard]] b2Transform get(const float t) const {
            const auto p = b2Lerp(transformBegin.p, transformEnd.p, t);
            const auto q = b2NLerp(transformBegin.q, transformEnd.q, t);
            return {p, q};
        }
    };

    class LocalTransformSpline {
    public:


        LocalSpline spline;
        EasingSpline easingSpeed{LinearEasing};
        EasingSpline easingAngle{LinearEasing};
        EasingSpline easingDim1{LinearEasing};
        EasingSpline easingDim2{LinearEasing};
        float startRadians{0};
        float endRadians{0};
        float startDim1{5};
        float startDim2{5};
        float endDim1{5};
        float endDim2{5};


        [[nodiscard]] b2Rot startAngle() const { return b2Rot(cos(startRadians), sin(startRadians)); }

        [[nodiscard]] b2Rot endAngle() const { return b2Rot(cos(endRadians), sin(endRadians)); }

        explicit LocalTransformSpline(LocalSpline localSpline) : spline(localSpline) {
        };

        LocalTransformSpline(const LocalSpline &spline, const EasingSpline &easing_speed,
                             const EasingSpline &easing_angle, const b2Rot &start_angle,
                             const b2Rot &end_angle);

        [[nodiscard]] b2Transform get(const float t) const {
            const auto bezier = spline.getLocalBezier();
            const auto eased_t = easingSpeed.valueAt(t);
            const Math::Vec2 p = bezier.valueAt(eased_t);
            const Math::Vec2 norm = bezier.normalAt(eased_t);
            if (isnan(norm.x) || isnan(norm.y)) {
                return {p, startAngle()};
            }
            const auto radians = atan2(norm.y, norm.x);
            const auto eased_angle_t = easingAngle.valueAt(t);

            const auto q = b2MulRot(b2Rot(cos(radians), sin(radians)),
                                    b2NLerp(startAngle(), endAngle(), eased_angle_t));
            return {p, q};
        }

        [[nodiscard]] float getDim1(const float t) const {
            return Lerp(startDim1, endDim1, easingDim1.valueAt(t));
        }

        [[nodiscard]] float getDim2(const float t) const {
            return Lerp(startDim2, endDim2, easingDim2.valueAt(t));
        }
    };

    struct BodyCouple {
        BodyCouple(const b2BodyId &owner, const b2BodyId &weapon)
                : owner(owner),
                  weapon(weapon) {
        }

        b2BodyId owner;
        b2BodyId weapon;
    };
}


#endif //ATTACKS_H
