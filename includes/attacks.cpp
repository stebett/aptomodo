//
// Created by ginko on 11/29/24.
//

#include "attacks.h"

#include <timer.h>
#include <systems/physics.h>

namespace Attacks {
    void Update(entt::registry &registry) {
        const auto now = GetTime();
        registry.view<Attacks::Attack>().each([&registry, now](auto entity, const auto attack) {
            auto timer{registry.get<PassiveTimer>(entity)};
            auto bodyCouple{registry.get<BodyCouple>(entity)};
            // auto oldTransform{registry.get<LocalTransform>(entity).get((now - timer.start) / timer.duration)};
            auto oldTransform{registry.get<LocalTransformSpline>(entity).get((now - timer.start) / timer.duration)};
            auto ownerTransform{b2Body_GetTransform(bodyCouple.owner)};
            auto newTransform = b2MulTransforms(ownerTransform, oldTransform);
            b2Body_SetTransform(bodyCouple.weapon, newTransform.p, newTransform.q);
            if (now - timer.start > timer.duration) {
                Physics::DestroyBody(bodyCouple.weapon);;
                registry.destroy(entity);
            }
        });
    }

    Attack::Attack(const float damage): damage(damage) {
    }
}
