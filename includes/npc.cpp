//
// Created by ginko on 02/03/24.
//

#include <entt/entity/registry.hpp>
#include <valarray>
#include <raylib.h>
#include "npc.h"
#include "constants.h"
#include <raymath.h>
#include "astar.h"
#include "collisions.h"
#include "rendering.h"
#include "config.h"

void
solveCollisionEnemy(const entt::registry &registry, const int id, Vector2 &futurePos, const float radius,
                    const Map &grid) {
    static Vector2 distance;
    static Vector2 enemyPos;
    static float overlap;
    auto enemyView = registry.view<Living, Radius, Position, ID>();
    for (auto [enemy, enemyRadius, enemyPosition, enemyID]: enemyView.each()) {
        if (enemyID.value == id) continue;
        enemyPos = {enemyPosition.x, enemyPosition.y};
        if (CheckCollisionCircles(futurePos, radius, enemyPos, enemyRadius.value)) {
            distance = Vector2Subtract(enemyPos, futurePos);
            overlap = radius + enemyRadius.value - Vector2Length(distance);
            if (overlap > 0) {
                futurePos = Vector2Subtract(futurePos, Vector2Scale(Vector2Normalize(distance), overlap));
                solveCircleRecCollision(futurePos, radius, grid);
            }
        }
    }
}


Triangle getTriangle(const Vector2 v1, const float range, const float angle1deg, const float angle2deg) {
    return {v1,
            {v1.x + range * cos(angle1deg * DEG2RAD),
             v1.y + range * sin(angle1deg * DEG2RAD)},
            {v1.x + range * cos(angle2deg * DEG2RAD),
             v1.y + range * sin(angle2deg * DEG2RAD)}};
}

//
//bool playerInView(const Vector2 &position, const Vector2 &playerPosition, const float radius, const float lookingAngleDeg) {
//    const float sightRange = 100.0f;
//    const float sightSpread = 45.0f;
//    Triangle triangle = getTriangle(position, sightRange, lookingAngleDeg - sightSpread, lookingAngleDeg + sightSpread);
//
//    bool inView = CheckCollisionPointTriangle(playerPosition, triangle.v1, triangle.v2, triangle.v3);
//    if (config::show_enemy_fov) {
//        DrawCircleSector(triangle.v1, sightRange, lookingAngleDeg - sightSpread, lookingAngleDeg + sightSpread, 2,
//                         ColorAlpha(WHITE, 0.1));
//        if (inView) { DrawCircle(position.x, position.y, 2, RED); }
//    }
//    return inView;
//}

bool playerInView(const Vector2 &position, const Vector2 &playerPosition, const float playerRadius,
                  const float lookingAngleDeg) {
    const float sightRange = 100.0f;
    Vector2 lookVector = {cos(lookingAngleDeg * DEG2RAD), sin(lookingAngleDeg * DEG2RAD)};
    bool facePlayer = Vector2DotProduct(lookVector, Vector2Subtract(playerPosition, position)) > 0;
    bool inViewRange = CheckCollisionCircles(playerPosition, playerRadius, position, sightRange);
    bool inView = facePlayer && inViewRange;
    if (config::show_enemy_fov) {
        DrawLineV(position, Vector2Add(position, Vector2Scale(lookVector, 20.0f)), PURPLE);
        DrawCircleSector(position, sightRange, lookingAngleDeg - 91.0f, lookingAngleDeg + 91.0f, 2,
                         ColorAlpha(WHITE, 0.1));
        if (inView) { DrawCircle(position.x + 15.0f, position.y + 15.0f, 2, RED); }
    }
    return inView;
}

void enemyAttack(entt::registry &registry, const entt::entity enemy, entt::entity player, Position &position) {
    static Triangle triangle;

    auto &attackTimer = registry.get<AttackTimer>(enemy).timer;
    if (attackTimer.Elapsed() < registry.get<AttackSpeed>(enemy).value) return;
    attackTimer.Reset();

    auto &playerPosition = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);
    auto radius = registry.get<Radius>(player);
    float attackRange = registry.get<AttackRange>(enemy).value;
    float attackSpread = registry.get<Spread>(enemy).value;
    float damage = registry.get<Damage>(enemy).value;
//    float pushback = registry.get<Pushback>(enemy).value;

    float clickAngle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;
    registry.emplace<AttackEffect>(registry.create(), 100, position, attackRange, clickAngle - attackSpread,
                                   clickAngle + attackSpread, BROWN);

    triangle = getTriangle(position, attackRange, clickAngle - attackSpread, clickAngle + attackSpread);
    if (CheckCollisionCircleTriangle(playerPosition, radius.value, triangle.v1, triangle.v2, triangle.v3,
                                     attackRange)) {
        health.value -= damage;
    }
}

void updatePath(entt::registry &registry, entt::entity &enemy, Position &position, Position &playerPosition,
                const Map &grid) {
    Path &path = registry.get<Path>(enemy);
    Node start = getTile(position);
    Node end = getTile(playerPosition);
    Search search(grid);
    search.init(start, end);

    // Check distance
    while (!search.completed) { search.step(); }

    if (search.path.empty()) { return; }
    if (config::show_astar_path) { search.draw(); }
    search.exportPath(path);
}

bool pathNextReached(const entt::registry &registry, const entt::entity &enemy) {
    Vector2 position = registry.get<Position>(enemy);
    const Path &path = registry.get<Path>(enemy);
    return Vector2Equals(position, path.getCurrent());
}


Vector2 getPathNext(entt::registry &registry, entt::entity &enemy) {
    Vector2 position = registry.get<Position>(enemy);
    Path &path = registry.get<Path>(enemy);
    Vector2 nextPath = pathNextReached(registry, enemy) ? path.getNext() : path.getCurrent();
    if (path.isFinished() || Vector2Equals(nextPath, Vector2Zero())) { return position; }

    if (config::show_astar_path) {
        DrawCircleV(nextPath, 2, DARKGREEN);
        DrawLineV(position, nextPath, GREEN);
    }
    return nextPath;
}


void faceTarget(const Vector2 &position, const Vector2 &target, const float turningRate, float &lookAngle) {

//    lookAngle = Lerp(lookAngle, atan2(target.y - position.y, target.x - position.x) * RAD2DEG, turningRate);
    lookAngle = atan2(target.y - position.y, target.x - position.x) * RAD2DEG;
}

void updatePosition(entt::registry &registry, entt::entity enemy, const int id, const float radius, const float speed,
                    const Map &grid, Position &position) {
    Vector2 target = getPathNext(registry, enemy);
    Vector2 direction = Vector2Subtract(target, position);
    Vector2 movement = Vector2Scale(Vector2Normalize(direction), speed);
    Vector2 futurePos = Vector2Add(position, movement);
    solveCollisionEnemy(registry, id, futurePos, radius, grid);
    solveCircleRecCollision(futurePos, radius, grid);
    position = futurePos;
}


bool playerInRange(const Vector2 &position, const Vector2 &playerPosition, const float radius) {
    return CheckCollisionCircles(playerPosition, radius, position, static_cast<float>(2 * tileSize));
}

void updateEnemy(entt::registry &registry, entt::entity &player, const Map &grid) {
    Position playerPosition = registry.get<Position>(player);
    float playerRadius = registry.get<Radius>(player).value;


    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID, LookAngle>();
    for (auto [enemy, speed, radius, health, position, id, lookAngle]: enemyView.each()) {
        if (health.value <= 0) {
            registry.remove<Living>(enemy);
            continue;
        }

        if (!playerInView(position, playerPosition, playerRadius, lookAngle.value)) {
            updatePosition(registry, enemy, id.value, radius.value, speed.value, grid, position);
            continue;
        }

        const float turningRate = 0.6f;
        faceTarget(position, playerPosition, turningRate, lookAngle.value);


        if (playerInRange(position, playerPosition, playerRadius)) {
            enemyAttack(registry, enemy, player, position);
            continue;
        }

        updatePath(registry, enemy, position, playerPosition, grid);
        updatePosition(registry, enemy, id.value, radius.value, speed.value, grid, position);

    }
}
