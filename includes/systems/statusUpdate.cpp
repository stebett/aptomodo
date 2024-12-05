//
// Created by ginko on 11/29/24.
//

#include "statusUpdate.h"
#include "managers/game.h"

#include <components.h>
#include <status.h>


namespace StatusEffect {
    void updateDash(double now) {
        Game::registry.view<StatusEffect::Dash>().each([now](auto entity, const StatusEffect::Dash &dash) {
            if (now - dash.timer.start > dash.timer.duration)
                Game::registry.remove<Dash>(entity);

            Speed &speed = Game::registry.get<Speed>(entity);

        });
    }

    void Update() {
        const auto now = GetTime();
        updateDash(now);
    }
}
