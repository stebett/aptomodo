//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H

#include "math/spline.h"

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
        std::string name {"default"};
        Spline trajectory;
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

        explicit Transform(Spline trajectory);;

        [[nodiscard]] b2Transform get(float t) const;

        [[nodiscard]] float getDim1(float t) const;

        [[nodiscard]] float getDim2(float t) const;

        void saveToTOML() const;

        void loadFromTOML(const std::string &name);

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
