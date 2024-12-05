//
// Created by ginko on 05/06/24.
//

#include "loop.h"
#include <components.h>
#include "strategies.h"
#include "managers/game.h"

namespace AI {
    void Update(const entt::entity player) {
        Game::registry.view<ToSimulate, Living, Enemy, Strategy::Strategy>().each([player](auto enemy, auto &strategy) {
            strategy.behavior->tick(enemy, player);
        });
    }
}
