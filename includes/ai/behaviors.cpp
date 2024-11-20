//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"

#include <collisions.h>
#include <components.h>
#include <config.h>
#include <constants.h>
#include <managers/animationManager.h>
#include <managers/audioManager.h>
#include <managers/levelManager.h>


Status PlayerInView::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInView" << "\n";
    const auto position = registry.get<Position>(self);
    const auto lookingAngleDeg = registry.get<LookAngle>(self);
    const auto playerPosition = registry.get<Position>(player);
    const auto playerRadius = registry.get<Radius>(player);
    const auto lookVector = Vector2{cos(lookingAngleDeg * DEG2RAD), sin(lookingAngleDeg * DEG2RAD)};

    const bool facePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) > 0;
    const bool inViewRange = CheckCollisionCircles(playerPosition, playerRadius, position, config::enemySightRange);
    const bool inHearRange = CheckCollisionCircles(playerPosition, playerRadius, position, config::enemyHearRange);
    const bool inView = (facePlayer && inViewRange) || inHearRange;

    if (config::show_enemy_fov) {
        DrawLineV(position, Vector2Add(position, Vector2Scale(lookVector, 20.0f)), PURPLE);
        DrawCircleSector(position, config::enemySightRange, lookingAngleDeg - 91.0f, lookingAngleDeg + 91.0f, 2,
                         ColorAlpha(WHITE, 0.1));
        DrawCircleV(position, config::enemyHearRange, ColorAlpha(WHITE, 0.1));
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
    const auto position = registry.get<Position>(self);
    const auto attackRange = registry.get<AttackRange>(self);
    const auto playerPosition = registry.get<Position>(player);
    const auto playerRadius = registry.get<Radius>(player);

    if (CheckCollisionCircles(position, attackRange, playerPosition, playerRadius))
        return SUCCESS;
    return FAILURE;
}

Status Chase::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Chase" << "\n";
    return FAILURE;
}

Triangle TriangleAngles(const Vector2 v1, const float range, const float angle1deg, const float angle2deg) {
    return {
        v1,
        {
            v1.x + range * cos(angle1deg * DEG2RAD),
            v1.y + range * sin(angle1deg * DEG2RAD)
        },
        {
            v1.x + range * cos(angle2deg * DEG2RAD),
            v1.y + range * sin(angle2deg * DEG2RAD)
        }
    };
}

Status AttackMelee::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "AttackMelee" << "\n";
    static Triangle triangle;
    const auto position = registry.get<Position>(self);

    auto &attackTimer = registry.get<AttackTimer>(self).timer;
    if (attackTimer.Elapsed() < registry.get<AttackSpeed>(self)) return RUNNING;
    attackTimer.Reset();

    auto &playerPosition = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);
    auto radius = registry.get<Radius>(player);
    auto attackRange = registry.get<AttackRange>(self);
    auto attackSpread = registry.get<Spread>(self);
    auto damage = registry.get<Damage>(self);
    //    float pushback = registry.get<Pushback>(enemy);

    float clickAngle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;
    registry.emplace<AttackEffect>(registry.create(), 100, position, attackRange, clickAngle - attackSpread,
                                   clickAngle + attackSpread, BROWN);
    AudioManager::Instance().Play("enemy_shot");


    triangle = TriangleAngles(position, attackRange, clickAngle - attackSpread, clickAngle + attackSpread);
    if (CheckCollisionCircleTriangle(playerPosition, radius, triangle.v1, triangle.v2, triangle.v3,
                                     attackRange)) {
        health -= damage;
    }
    return SUCCESS;
}

bool emplaceRandomTarget(entt::registry &registry, entt::entity self) {
    const auto position = registry.get<Position>(self);
    for (int x = 0, y = config::enemyPatrolDistance; x <= config::enemyPatrolDistance; x++, y--) {
        if (LevelManager::grid(static_cast<int>(floor(position.x / tileSize)) + x,
                               static_cast<int>(floor(position.y / tileSize)) + y) == -1) {
            Target target = {
                position.x + static_cast<float>(x * tileSize),
                position.y + static_cast<float>(y * tileSize)
            };
            registry.emplace<Target>(self, target);
            return true;
        }
    }
    return false;
}

Status GetRandomTarget::update(entt::registry &registry, entt::entity self, entt::entity player) {
    if (!registry.all_of<Target>(self)) {
        if (emplaceRandomTarget(registry, self)) {
            return SUCCESS;
        }
        return FAILURE;
    }
    if (config::show_astar_path) {
        Vector2 target = registry.get<Target>(self);
        DrawCircleV(target, 2, YELLOW);
    }

    return SUCCESS;
}

Status MoveTowardsTarget::update(entt::registry &registry, entt::entity self, entt::entity player) {
    auto &path = registry.get<Path>(self);

    return FAILURE;
}
