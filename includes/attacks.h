//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H
#include <math/spline.h>


namespace Attacks {
    struct Hit {
    };

    void Update(entt::registry &registry);

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

        explicit LocalTransformSpline(LocalSpline localSpline) : spline(localSpline) {
        };


        [[nodiscard]] b2Transform get(const float t) const {
            const auto points = spline.getLocal();
            const auto p = b2Lerp({points[0].x, points[0].y},
                                  {points[3].x, points[3].y}, t);
            const auto q = b2Rot{1, 0};
            return {p, q};
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
