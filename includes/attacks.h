//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H


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
        b2Transform transform;

        explicit LocalTransform(b2Transform transform) : transform(transform) {};

        [[nodiscard]] const b2Transform& get(float t) const {
            return transform;
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
