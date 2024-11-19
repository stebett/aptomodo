//
// Created by ginko on 05/06/24.
//

#include "loop.h"
#include <components.h>
#include "strategies.h"

namespace AI {
    void Update(entt::registry &registry, entt::entity &player) {
        auto enemyView = registry.view<Living, Enemy, Strategy::Strategy>();
        for (auto [enemy, strategy]: enemyView.each()) {
            strategy.behavior->tick();
        }
    }
}
