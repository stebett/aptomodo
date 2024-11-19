//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"

#include <components.h>
#include <config.h>


Status PlayerInView::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInView" << "\n";
    auto position = registry.get<Position>(self);
    auto lookingAngleDeg = registry.get<LookAngle>(self);
    auto chasing = registry.all_of<Chasing>(self);
    auto playerPosition = registry.get<Position>(player);
    auto playerRadius = registry.get<Radius>(player);
    const float sightRange = chasing ? config::enemySightRangeChasing : config::enemySightRange;
    const float hearRange = chasing ? config::enemyHearRangeChasing : config::enemyHearRange;

    Vector2 lookVector = {cos(lookingAngleDeg * DEG2RAD), sin(lookingAngleDeg * DEG2RAD)};
    bool facePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) > 0;
    bool inViewRange = CheckCollisionCircles(playerPosition, playerRadius, position, sightRange);
    bool inHearRange = CheckCollisionCircles(playerPosition, playerRadius, position, hearRange);
    bool inView = (facePlayer && inViewRange) || inHearRange;
    if (config::show_enemy_fov) {
        DrawLineV(position, Vector2Add(position, Vector2Scale(lookVector, 20.0f)), PURPLE);
        DrawCircleSector(position, sightRange, lookingAngleDeg - 91.0f, lookingAngleDeg + 91.0f, 2,
                         ColorAlpha(WHITE, 0.1));
        DrawCircleV(position, hearRange, ColorAlpha(WHITE, 0.1));
        if (inView) { DrawCircle(position.x + 15.0f, position.y + 15.0f, 2, RED); }
    }
    return SUCCESS;

}

Status Patrol::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Patrol" << "\n";
    return SUCCESS;
}

Status PlayerInMeleeRange::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInMeleeRange" << "\n";
    return SUCCESS;
}

Status Chase::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Chase" << "\n";
    return SUCCESS;
}

Status AttackMelee::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "AttackMelee" << "\n";
    return SUCCESS;
}
