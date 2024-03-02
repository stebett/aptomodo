//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <valarray>
#include "npc.h"

void moveTowardsPoint(Position &position, Position &target) {
    float speed = 4.0f;
    float m = sqrt(pow(position.x - target.x, 2) + pow(position.y - target.y, 2));
    position = {(position.x - (position.x - target.x) * speed / m),
                position.y - (position.y - target.y) * speed / m};
//    position.y -= speed * static_cast<float>(position.y > target.y);
//    position.y += speed * static_cast<float>(position.y < target.y);
//    position.x -= speed * static_cast<float>(position.x > target.x);
//    position.x += speed * static_cast<float>(position.x < target.x);
}


void updateEnemy(entt::registry &registry, Position &playerPosition) {
    auto enemyView = registry.view<Living, Health, Position, Enemy>();
    for (auto [enemy, health, position]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            return;
        }
        moveTowardsPoint(position, playerPosition);
    }
}
