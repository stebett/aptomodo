//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"

#include <astar.h>
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
    const auto chasing = registry.get<Chasing>(self);

    const bool facePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) > 0;
    const bool inViewRange = CheckCollisionCircles(playerPosition, playerRadius, position, config::enemySightRange);
    const bool inHearRange = CheckCollisionCircles(playerPosition, playerRadius, position, config::enemyHearRange);
    const bool inViewChasingRange = CheckCollisionCircles(playerPosition, playerRadius, position,
                                                          config::enemySightRangeChasing);
    const bool inHearChasingRange = CheckCollisionCircles(playerPosition, playerRadius, position,
                                                          config::enemyHearRangeChasing);
    const bool inView = (facePlayer && inViewRange) || inHearRange;
    const bool inChasingView = (facePlayer && inViewChasingRange) || inHearChasingRange;
    const bool result = chasing.isChasing() ? inChasingView : inView;

    if (result)
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


bool emplaceRandomTarget(entt::registry &registry, entt::entity self) {
    const auto position = registry.get<Position>(self);
    constexpr int num_points = 20;
    std::set<Target> uniquePoints;
    std::vector<Target> sampledPoints;

    for (int i = 0; i < num_points; ++i) {
        const auto theta = static_cast<float>(2.0 * M_PI * i / num_points);
        const float x = round(position.x + config::enemyPatrolDistance * cos(theta));
        const float y = round(position.y + config::enemyPatrolDistance * sin(theta));
        if (LevelManager::grid.inWorldBounds(x, y) &&
            LevelManager::grid.fromWorld(x, y) == IntValue::EMPTY) {
            uniquePoints.insert(Target{x, y});
        }
    }
    std::ranges::sample(uniquePoints, std::back_inserter(sampledPoints),
                        1, std::mt19937{std::random_device{}()});

    if (sampledPoints.empty()) return false;
    registry.emplace_or_replace<Target>(self, sampledPoints[0]);
    return true;
}

bool reachedTile(const Vector2 &position, const Vector2 &target) {
    return LevelManager::grid.fromWorld(position) == LevelManager::grid.fromWorld(target) ||
           Vector2Length(Vector2Subtract(position, target)) < 10.0f;;
}

Status GetRandomTarget::update(entt::registry &registry, entt::entity self, entt::entity player) {
    const auto position = registry.get<Position>(self);

    if (!registry.all_of<Target>(self) || reachedTile(position, registry.get<Target>(self))) {
        if (emplaceRandomTarget(registry, self)) {
            return SUCCESS;
        }
        return FAILURE;
    }
    return SUCCESS;
}

Status GetPlayerTarget::update(entt::registry &registry, entt::entity self, entt::entity player) {
    const auto playerPosition = registry.get<Position>(player);
    registry.emplace_or_replace<Target>(self, playerPosition);
    registry.get<Chasing>(self).timer.Reset();
    registry.get<Path>(self).invalidate();;


    return SUCCESS;
}

void adjustLookAngle(const Vector2 &position, const Vector2 &target, LookAngle &lookAngle, float step = 8.0f) {
    // Compute the target angle in degrees
    float targetAngle = atan2(target.y - position.y, target.x - position.x) * RAD2DEG;

    // Normalize angles to the range [0, 360)
    lookAngle = fmod(lookAngle + 360.0f, 360.0f);
    targetAngle = fmod(targetAngle + 360.0f, 360.0f);

    // Compute the shortest direction to move (clockwise or counterclockwise)
    float delta = targetAngle - lookAngle;
    if (delta > 180.0f) delta -= 360.0f; // Shortest path counterclockwise
    if (delta < -180.0f) delta += 360.0f; // Shortest path clockwise

    // Clamp the adjustment to the step size
    if (std::abs(delta) <= step) {
        lookAngle = targetAngle; // Close enough, snap to the target
    } else {
        lookAngle = lookAngle + (delta > 0 ? step : -step); // Move 5 degrees in the correct direction
    }

    // Normalize the result to [0, 360)
    lookAngle = fmod(lookAngle + 360.0f, 360.0f);
}

bool facesTargetDirection(const Vector2 &position, const Vector2 &target, const LookAngle &lookAngle) {
    return abs(lookAngle - fmod(atan2(target.y - position.y, target.x - position.x) * RAD2DEG + 360.0f, 360.0f)) < 0.1;
}

void faceDirection(const Vector2 &position, const Vector2 &target, LookAngle &lookAngle) {
    //    lookAngle = Lerp(lookAngle, atan2(target.y - position.y, target.x - position.x) * RAD2DEG, turningRate);
    lookAngle = atan2(target.y - position.y, target.x - position.x) * RAD2DEG;
}

void solveCollisionEnemyEnemy(const entt::registry &registry, const int id, Vector2 &futurePos, const float radius) {
    static Vector2 distance;
    static Vector2 enemyPos;
    static float overlap;
    auto enemyView = registry.view<Living, Radius, Position, ID>();
    for (auto [enemy, enemyRadius, enemyPosition, enemyID]: enemyView.each()) {
        if (enemyID == id) continue;
        enemyPos = {enemyPosition.x, enemyPosition.y};
        if (CheckCollisionCircles(futurePos, radius, enemyPos, enemyRadius)) {
            distance = Vector2Subtract(enemyPos, futurePos);
            overlap = radius + enemyRadius - Vector2Length(distance);
            if (overlap > 0) {
                futurePos = Vector2Subtract(futurePos, Vector2Scale(Vector2Normalize(distance), overlap));
                solveCircleRecCollision(futurePos, radius);
            }
        }
    }
}

Status MoveTowardsTarget::update(entt::registry &registry, entt::entity self, entt::entity player) {
    Path &path = registry.get<Path>(self);
    auto &position = registry.get<Position>(self);

    if (!path.isValid()) {
        Search search;
        const auto target = registry.get<Target>(self);
        const Node start = getTile(position);
        const Node end = getTile(target);
        search.init(start, end);
        while (!search.completed) { search.step(); }
        if (search.path.empty()) { return FAILURE; }
        search.exportPath(path);
    }
    if (path.indexMax == 0) {
        registry.remove<Target>(self);
        return FAILURE;
    }
    const Vector2 nextTarget = reachedTile(position, path.getCurrent()) ? path.getNext() : path.getCurrent();
    const auto radius = registry.get<Radius>(self);
    const auto id = registry.get<ID>(self);
    auto &lookAngle = registry.get<LookAngle>(self);
    auto speed = registry.get<Speed>(self);
    const Vector2 direction = Vector2Subtract(nextTarget, position);
    const Vector2 movement = Vector2Scale(Vector2Normalize(direction), speed);
    Vector2 futurePos = Vector2Add(position, movement);
    solveCollisionEnemyEnemy(registry, id, futurePos, radius);
    solveCircleRecCollision(futurePos, radius);
    if (!facesTargetDirection(position, futurePos, lookAngle)) {
        adjustLookAngle(position, futurePos, lookAngle);
        futurePos = position;
    }

    speed.actual = Vector2Distance(position, futurePos);
    position = futurePos;
    return SUCCESS;
}

Status AttackMelee::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "AttackMelee" << "\n";
    static Triangle triangle;
    const auto position = registry.get<Position>(self);
    const auto &playerPosition = registry.get<Position>(player);
    auto &lookAngle = registry.get<LookAngle>(self);

    if (!facesTargetDirection(position, playerPosition, lookAngle)) {
        adjustLookAngle(position, playerPosition, lookAngle);
        return FAILURE;
    }
    auto &attackTimer = registry.get<AttackTimer>(self).timer;
    if (attackTimer.ElapsedSeconds() < registry.get<AttackSpeed>(self)) return RUNNING;
    attackTimer.Reset();


    auto &health = registry.get<Health>(player);
    const auto radius = registry.get<Radius>(player);
    const auto attackRange = registry.get<AttackRange>(self);
    const auto attackSpread = registry.get<Spread>(self);
    const auto damage = registry.get<Damage>(self);
    //    float pushback = registry.get<Pushback>(enemy);


    const float clickAngle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;
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
