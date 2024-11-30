//
// Created by ginko on 11/29/24.
//

#include "attacks.h"

#include <timer.h>

namespace Attacks {
    void Update(entt::registry &registry) {
        const auto now = std::chrono::high_resolution_clock::now();
        registry.view<Attacks::Attack>().each([&registry, now](auto entity, const auto attack) {
            auto timer = registry.get<PassiveTimer>(entity);
            if (now - timer.start  > timer.duration) {
                auto body = registry.get<b2BodyId>(entity);
                b2DestroyBody(body);
                registry.destroy(entity);
            }

        });    }

    Attack::Attack(const float damage): damage(damage) {
    }
}
