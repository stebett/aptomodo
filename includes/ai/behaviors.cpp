//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"

#include "astar.h"
#include <collisions.h>
#include <components.h>
#include "managers/configManager.h"
#include <managers/animationManager.h>
#include <managers/audioManager.h>
#include <math/mathConstants.h>
#include <systems/space.h>

#include "shadowCast.h"
#include "managers/game.h"


Status PlayerInView::update(entt::entity self, entt::entity player) {
    const auto position = Game::registry.get<Position>(self);
    const auto lookingAngleDeg = Game::registry.get<LookAngle>(self);
    const auto playerPosition = Game::registry.get<Position>(player);
    const auto playerRadius = Game::registry.get<Radius>(player);
    const auto lookVector = Vector2{cos(lookingAngleDeg * DEG2RAD), sin(lookingAngleDeg * DEG2RAD)};
    auto &chasing = Game::registry.get<Chasing>(self);
    const bool isChasing = chasing.isChasing();

    const auto hearRange = isChasing ? Config::GetFloat("enemyHearRangeChasing") : Config::GetFloat("enemyHearRange");
    const auto sightRange = isChasing
                            ? Config::GetFloat("enemySightRangeChasing")
                            : Config::GetFloat("enemySightRange");
    assert(hearRange < sightRange && "Sight range must be bigger then hear range, or you have to change this logic");
    if (const bool inHearRange = CheckCollisionCircles(playerPosition, playerRadius, position, hearRange)) {
        chasing.timer.Reset();
        return SUCCESS;
    }
    if (const bool notFacePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) < 0) {
        return FAILURE;
    }
    if (const bool outSightRange = !CheckCollisionCircles(playerPosition, playerRadius, position, sightRange)) {
        return FAILURE;
    }
    if (const bool inFOV = isTargetInFOV(Game::grid, position.x, position.y, playerPosition.x,
                                         playerPosition.y, sightRange, lookingAngleDeg - 90.0f,
                                         lookingAngleDeg + 90.0f)) {
        chasing.timer.Reset();
        return SUCCESS;
    }

    return FAILURE;
}


Status PlayerInMeleeRange::update(entt::entity self, entt::entity player) {
    const auto position = Game::registry.get<Position>(self);
    const auto attackRange = Game::registry.get<AttackRange>(self);
    const auto playerPosition = Game::registry.get<Position>(player);
    const auto playerRadius = Game::registry.get<Radius>(player);

    if (CheckCollisionCircles(position, attackRange, playerPosition, playerRadius))
        return SUCCESS;
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


bool emplaceRandomTarget(entt::entity self) {
    const auto position = Game::registry.get<Position>(self);
    constexpr int num_points = 20;
    std::set<Target> uniquePoints;
    std::vector<Target> sampledPoints;

    for (int i = 0; i < num_points; ++i) {
        const auto theta = static_cast<float>(2.0 * M_PI * i / num_points);
        const float x = round(position.x + Config::GetFloat("enemyPatrolDistance") * cos(theta));
        const float y = round(position.y + Config::GetFloat("enemyPatrolDistance") * sin(theta));
        if (Game::grid.inWorldBounds(x, y) &&
            Game::grid.fromWorld(x, y)[IntValue::EMPTY]) {
            uniquePoints.insert(Target{x, y});
        }
    }
    std::ranges::sample(uniquePoints, std::back_inserter(sampledPoints),
                        1, std::mt19937{std::random_device{}()});

    if (sampledPoints.empty()) return false;
    Game::registry.emplace_or_replace<Target>(self, sampledPoints[0]);
    return true;
}

bool reachedTile(const Vector2 &position, const Vector2 &target) {
    return Vector2Length(Vector2Subtract(position, target)) < 5.0f;;
}

Status GetRandomTarget::update(entt::entity self, entt::entity player) {
    const auto position = Game::registry.get<Position>(self);

    if (Game::registry.all_of<Target>(self) || reachedTile(position, Game::registry.get<Target>(self))) {
        if (emplaceRandomTarget(self)) {
            return SUCCESS;
        }
        return FAILURE;
    }
    return SUCCESS;
}

Status GetPlayerTarget::update(entt::entity self, entt::entity player) {
    const auto playerPosition = Game::registry.get<Position>(player);
    Game::registry.emplace_or_replace<Target>(self, playerPosition);
    Game::registry.get<Path>(self).invalidate();;


    return SUCCESS;
}

void adjustLookAngle(const Vector2 &target, LookAngle &lookAngle, float step = 8.0f) {
    // Compute the target angle in degrees
    float targetAngle = atan2(target.y, target.x) * RAD2DEG;

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

bool facesTargetDirection(const Vector2 &target, const LookAngle &lookAngle) {
    return abs(lookAngle - fmod(atan2(target.y, target.x) * RAD2DEG + 360.0f, 360.0f)) < 0.1;
}

void faceDirection(const Vector2 &target, LookAngle &lookAngle) {
    //    lookAngle = Lerp(lookAngle, atan2(target.y - position.y, target.x - position.x) * RAD2DEG, turningRate);
    lookAngle = atan2(target.y, target.x) * RAD2DEG;
}

void solveCollisionEnemyEnemy(const int id, Vector2 &futurePos, const float radius) {
    static Vector2 distance;
    static Vector2 enemyPos;
    static float overlap;
    auto enemyView = Game::registry.view<Living, Radius, Position, ID>();
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

Status MoveTowardsTarget::update(entt::entity self, entt::entity player) {
    Path &path = Game::registry.get<Path>(self);
    auto &position = Game::registry.get<Position>(self);
    auto &radius = Game::registry.get<Radius>(self);

    if (!path.isValid()) {
        Search search;
        const auto target = Game::registry.get<Target>(self);
        const Node start = getTile(position);
        const Node end = getTile(target);
        search.init(start, end);
        std::unordered_set<Node> tileSet{};
        for (const auto v: Space::tilesOccupied(position, radius))
            tileSet.insert(Node(v));
        search.setFree(tileSet);

        while (!search.completed) { search.step(); }
        if (search.path.empty()) { return FAILURE; }
        search.exportPath(path);
    }
    if (path.indexMax == 0) {
        Game::registry.remove<Target>(self);
        return FAILURE;
    }
    const Vector2 nextTarget = reachedTile(position, path.getCurrent()) ? path.getNext() : path.getCurrent();

    auto &lookAngle = Game::registry.get<LookAngle>(self);
    const float speedDivider = Game::registry.get<Chasing>(self).isChasing() ? 1.0f : 3.0f;
    auto &speed = Game::registry.get<Speed>(self);
    speed.value = speed.max / speedDivider;
    const Vector2 direction = Vector2Subtract(nextTarget, position);

    const auto body = Game::registry.get<b2BodyId>(self);
    constexpr int framerateCorrection = 60;
    const Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed * framerateCorrection);
    b2Body_SetLinearVelocity(body, {velocity.x, velocity.y});
    adjustLookAngle(velocity, lookAngle);

    return SUCCESS;
}

Status AttackMelee::update(entt::entity self, entt::entity player) {
    static Triangle triangle;
    const auto position = Game::registry.get<Position>(self);
    const auto &playerPosition = Game::registry.get<Position>(player);
    auto &lookAngle = Game::registry.get<LookAngle>(self);
    adjustLookAngle(Vector2Subtract(playerPosition, position), lookAngle);

    auto &attackTimer = Game::registry.get<AttackTimer>(self).timer;
    if (attackTimer.ElapsedSeconds() < Game::registry.get<AttackSpeed>(self)) return RUNNING;
    attackTimer.Reset();


    auto &health = Game::registry.get<Health>(player);
    const auto radius = Game::registry.get<Radius>(player);
    const auto attackRange = Game::registry.get<AttackRange>(self);
    const auto attackSpread = Game::registry.get<Spread>(self);
    const auto damage = Game::registry.get<Damage>(self);
    //    float pushback = Game::registry.get<Pushback>(enemy);


    const float clickAngle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) *
                             Math::Const::radToDeg;
    Game::registry.emplace<AttackEffect>(Game::registry.create(), 100, position, attackRange, clickAngle - attackSpread,
                                         clickAngle + attackSpread, BROWN);
    Game::registry.emplace<Audio::Command>(Game::registry.create(), "enemy_shot");


    triangle = TriangleAngles(position, attackRange, clickAngle - attackSpread, clickAngle + attackSpread);
    if (CheckCollisionCircleTriangle(playerPosition, radius, triangle.v1, triangle.v2, triangle.v3,
                                     attackRange)) {
        health -= damage;
    }
    return SUCCESS;
}
