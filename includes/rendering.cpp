//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include "rendering.h"
#include "components.h"
#include <raylib.h>


void draw(const entt::registry &registry) {
    auto playerView = registry.view<Player, Living, Radius, Position>();
    for (auto [entity, radius, position]: playerView.each()) {
        DrawCircle(position.x, position.y, radius.value, RED);
    }
    auto enemyView = registry.view<Enemy, Living, Radius, Position>();
    for (auto [entity, radius, position]: enemyView.each()) {
        DrawCircle(position.x, position.y, radius.value, BROWN);
    }
}
