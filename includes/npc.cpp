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
#include "audioManager.h"

void
solveCollisionEnemy(const entt::registry &registry, const int id, Vector2 &futurePos, const float radius,
                    const Map &grid) {
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


bool playerInView(const Vector2 &position, const Vector2 &playerPosition, const float playerRadius,
                  const float lookingAngleDeg) {
    const float sightRange = 100.0f; // TODO make it bigger when chasing
    const float hearRange = 40.0f;
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
    return inView;
}

void enemyAttack(entt::registry &registry, const entt::entity enemy, entt::entity player, Position &position) {
    static Triangle triangle;

    auto &attackTimer = registry.get<AttackTimer>(enemy).timer;
    if (attackTimer.Elapsed() < registry.get<AttackSpeed>(enemy)) return;
    attackTimer.Reset();

    auto &playerPosition = registry.get<Position>(player);
    auto &health = registry.get<Health>(player);
    auto radius = registry.get<Radius>(player);
    auto attackRange = registry.get<AttackRange>(enemy);
    auto attackSpread = registry.get<Spread>(enemy);
    auto damage = registry.get<Damage>(enemy);
//    float pushback = registry.get<Pushback>(enemy);

    float clickAngle = atan2(playerPosition.y - position.y, playerPosition.x - position.x) * radToDeg;
    registry.emplace<AttackEffect>(registry.create(), 100, position, attackRange, clickAngle - attackSpread,
                                   clickAngle + attackSpread, BROWN);
    AudioManager::Instance().Play("enemy_shot");


    triangle = getTriangle(position, attackRange, clickAngle - attackSpread, clickAngle + attackSpread);
    if (CheckCollisionCircleTriangle(playerPosition, radius, triangle.v1, triangle.v2, triangle.v3,
                                     attackRange)) {
        health -= damage;
    }
}

void updatePath(entt::registry &registry, entt::entity &enemy, Position &position, Position &playerPosition,
                const Map &grid) {
    Path &path = registry.get<Path>(enemy);
    if (!path.isFinished())
        if (!registry.all_of<Chasing>(enemy))
                return;
    registry.remove<Chasing>(enemy);
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


Position getRandomPos(Position &position) {
    Vector2 target = Vector2Scale({rng::uniform_neg500_500(rng::seed) / 500.0f,
                                   rng::uniform_neg500_500(rng::seed) / 500.0f}, 100);

    return Vector2Add(position, target);
}

bool pathCurrentReached(const Vector2 &position, const Vector2 &currentTarget, const float speed) {
    return Vector2Length(Vector2Subtract(position, currentTarget)) < speed;
}


Vector2 getPathNext(entt::registry &registry, entt::entity &enemy) {
    Vector2 position = registry.get<Position>(enemy);
    Path &path = registry.get<Path>(enemy);
    const Vector2 currentTarget = path.getCurrent();
    const float speed = registry.get<Speed>(enemy);
    Vector2 nextPath = pathCurrentReached(position, currentTarget, speed) ? path.getNext() : currentTarget;
//    if (path.isFinished() || Vector2Equals(nextPath, Vector2Zero())) { return position; }
    if (path.isFinished()) { return position; }

    if (config::show_astar_path) {
        DrawCircleV(nextPath, 2, DARKGREEN);
        DrawLineV(position, nextPath, GREEN);
    }
    return nextPath;
}


void faceTarget(const Vector2 &position, const Vector2 &target, LookAngle &lookAngle) {

//    lookAngle = Lerp(lookAngle, atan2(target.y - position.y, target.x - position.x) * RAD2DEG, turningRate);
    lookAngle = atan2(target.y - position.y, target.x - position.x) * RAD2DEG;
}

void updatePosition(entt::registry &registry, entt::entity enemy, const int id, const float radius, const float speed,
                    const Map &grid, Position &position, LookAngle &lookAngle) {
    Vector2 target = getPathNext(registry, enemy);
    if (Vector2Equals(position, target)) { return; }
    Vector2 direction = Vector2Subtract(target, position);
    Vector2 movement = Vector2Scale(Vector2Normalize(direction), speed);
    Vector2 futurePos = Vector2Add(position, movement);
    solveCollisionEnemy(registry, id, futurePos, radius, grid);
    solveCircleRecCollision(futurePos, radius, grid);
    faceTarget(position, futurePos, lookAngle);
    position = futurePos;
}


bool playerInRange(const Vector2 &position, const Vector2 &playerPosition, const float radius) {
    return CheckCollisionCircles(playerPosition, radius, position, static_cast<float>(2 * tileSize));
}

void updateEnemy(entt::registry &registry, entt::entity &player, const Map &grid) {
    Position playerPosition = registry.get<Position>(player);
    float playerRadius = registry.get<Radius>(player);
    const float turningRate = 0.6f;

    auto enemyView = registry.view<Living, Speed, Radius, Health, Position, Enemy, ID, LookAngle>();
    for (auto [enemy, speed, radius, health, position, id, lookAngle]: enemyView.each()) {
        if (health <= 0) {
            registry.remove<Living>(enemy);
            AudioManager::Instance().Play("enemy_explosion");
            continue;
        }

        if (!playerInView(position, playerPosition, playerRadius, lookAngle)) {
            Position randomPos = getRandomPos(position);
            float speedDivider = registry.all_of<Chasing>(enemy) ? 1.0f : 5.0f;
            speed.value = speed.max / speedDivider;
            updatePath(registry, enemy, position, randomPos, grid);
            updatePosition(registry, enemy, id, radius, speed, grid, position, lookAngle);
            continue;
        }
        registry.emplace_or_replace<Chasing>(enemy);
        speed.value = speed.max;


        if (playerInRange(position, playerPosition, playerRadius)) {
            faceTarget(position, playerPosition, lookAngle);
            enemyAttack(registry, enemy, player, position);
            continue;
        }

        updatePath(registry, enemy, position, playerPosition, grid);
        updatePosition(registry, enemy, id, radius, speed, grid, position, lookAngle);

    }
}
