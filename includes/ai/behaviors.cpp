//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"

#include <components.h>
#include <config.h>


Status PlayerInView::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInView" << "\n";
    const auto position = registry.get<Position>(self);
    const auto lookingAngleDeg = registry.get<LookAngle>(self);
    const auto chasing = registry.all_of<Chasing>(self);
    const auto playerPosition = registry.get<Position>(player);
    const auto playerRadius = registry.get<Radius>(player);
    const float sightRange = chasing ? config::enemySightRangeChasing : config::enemySightRange;
    const float hearRange = chasing ? config::enemyHearRangeChasing : config::enemyHearRange;

    const Vector2 lookVector = {cos(lookingAngleDeg * DEG2RAD), sin(lookingAngleDeg * DEG2RAD)};
    const bool facePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) > 0;
    const bool inViewRange = CheckCollisionCircles(playerPosition, playerRadius, position, sightRange);
    const bool inHearRange = CheckCollisionCircles(playerPosition, playerRadius, position, hearRange);
    const bool inView = (facePlayer && inViewRange) || inHearRange;
    if (config::show_enemy_fov) {
        DrawLineV(position, Vector2Add(position, Vector2Scale(lookVector, 20.0f)), PURPLE);
        DrawCircleSector(position, sightRange, lookingAngleDeg - 91.0f, lookingAngleDeg + 91.0f, 2,
                         ColorAlpha(WHITE, 0.1));
        DrawCircleV(position, hearRange, ColorAlpha(WHITE, 0.1));
        if (inView) { DrawCircle(static_cast<int>(position.x) + 15, static_cast<int>(position.y) + 15, 2, RED); }
    }
    if (inView)
        return SUCCESS;

    return FAILURE;
}

Status Patrol::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Patrol" << "\n";

    return FAILURE;
}

Status PlayerInMeleeRange::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInMeleeRange" << "\n";
    return FAILURE;
}

Status Chase::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Chase" << "\n";
    return FAILURE;
}

Status AttackMelee::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "AttackMelee" << "\n";
    return FAILURE;
}
