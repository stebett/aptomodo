//
// Created by ginko on 05/06/24.
//

#include "loop.h"
#include <components.h>
#include "strategies.h"

namespace AI {
    void Update(entt::registry &registry, entt::entity &player) { // TODO this doesnt need to be a reference to player, just value is good
        auto enemyView = registry.view<Living, Enemy, Strategy::Strategy>();
        // TODO need to simulate only enemies in AI area. need something like bool Space::inAISpace(entity) {...}
        // TODO Or maybe we can have a SpaceSystem that iterates and tags all the enemies that are to render and simulate, to avoid recomputing this more than once
        for (auto [enemy, strategy]: enemyView.each()) {
            strategy.behavior->tick(registry, enemy, player);
        }
    }
}
