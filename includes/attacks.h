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


    class Transform {
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


        [[nodiscard]] b2Rot startAngle() const;

        [[nodiscard]] b2Rot endAngle() const;

        explicit Transform(LocalSpline localSpline);;

        [[nodiscard]] b2Transform get(const float t) const;

        [[nodiscard]] float getDim1(const float t) const;

        [[nodiscard]] float getDim2(const float t) const;
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
