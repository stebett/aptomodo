//
// Created by ginko on 05/06/24.
//

#include "loop.h"
#include <components.h>
#include "strategies.h"

namespace AI {
    void Update(entt::registry &registry, const entt::entity player) {
       registry.view<ToSimulate, Living, Enemy, Strategy::Strategy>().each([&registry, player](auto enemy, auto& strategy) {
           strategy.behavior->tick(registry, enemy, player);
        });
        // TODO need to simulate only enemies in AI area. need something like bool Space::inAISpace(entity) {...}
        // TODO Or maybe we can have a SpaceSystem that iterates and tags all the enemies that are to render and simulate, to avoid recomputing this more than once
    }
}
