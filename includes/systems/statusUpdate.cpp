//
// Created by ginko on 11/29/24.
//

#include "statusUpdate.h"

#include <components.h>
#include <status.h>


namespace StatusEffect {
    void updateDash(entt::registry &registry, double now) {
        registry.view<StatusEffect::Dash>().each([&registry, now](auto entity, const StatusEffect::Dash &dash) {
            if (now - dash.timer.start  > dash.timer.duration)
                registry.remove<Dash>(entity);

            Speed &speed = registry.get<Speed>(entity);

        });
    }

    void Update(entt::registry &registry) {
        const auto now = GetTime();
        updateDash(registry, now);
    }
}
