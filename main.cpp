#include <raylib.h>
#include <entt/entity/registry.hpp>
#include <iostream>
#include <random>
#include "includes/components.h"


const int screenWidth = 800;
const int screenHeight = 600;
const float radToDeg = (180.0 / 3.141592653589793238463);
const float degToRad = (3.141592653589793238463 / 180.0);

Weapon sword{10, 100, 25, 150};

float distanceBetweenPoints(Position p1, Position p2) {
    return static_cast<float>(sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
}

bool CheckCollisionLineCircle(Vector2 start, Vector2 end, Vector2 center, float radius) {
    if (CheckCollisionPointCircle(end, center, radius)) return true;
    float distX = start.x - end.x;
    float distY = start.y - end.y;
    float len = sqrt((distX * distX) + (distY * distY));
    float dot = (((center.x - start.x) * (end.x - start.x)) + ((center.y - start.y) * (end.y - start.y))) / pow(len, 2);
    float closestX = start.x + (dot * (end.x - start.x));
    float closestY = start.y + (dot * (end.y - start.y));
    bool onSegment = CheckCollisionPointLine(Vector2{closestX, closestY}, start, end, 1);
    if (!onSegment) return false;
    distX = closestX - center.x;
    distY = closestY - center.y;
    float distance = sqrt((distX * distX) + (distY * distY));
    if (distance <= radius) {
        return true;
    }
    return false;
}

bool CheckTriangleContainsCircle(Vector2 center, Vector2 v1, Vector2 v2, Vector2 v3) {
    if ((((v2.y - v1.y) * (center.x - v1.x) - (v2.x - v1.x) * (center.y - v1.y)) <= 0)
        &&
        (((v3.y - v2.y) * (center.x - v2.x) - (v3.x - v2.x) * (center.y - v2.y)) <= 0)
        &&
        (((v1.y - v3.y) * (center.x - v3.x) - (v1.x - v3.x) * (center.x - v3.x)) <= 0))
        return true;
    return false;
}


bool
CheckCollisionCircleTriangle(Vector2 center, float radius, Vector2 v1, Vector2 v2, Vector2 v3, float height) {
    if (!CheckCollisionCircles(center, radius, v1, height)) return false;
    if (CheckCollisionLineCircle(v1, v2, center, radius)) return true;
    if (CheckCollisionLineCircle(v1, v3, center, radius)) return true;
    if (CheckCollisionLineCircle(v2, v3, center, radius)) return true;
    if (CheckTriangleContainsCircle(center, v1, v2, v3)) return true;

    return false;
}


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

void playerAttack(entt::registry &registry, entt::entity player, Vector2 clickPosition) {
    Position playerPosition = registry.get<Position>(player);
    Weapon playerWeapon = registry.get<Weapon>(player);

    float click_angle = atan2(clickPosition.y - playerPosition.y, clickPosition.x - playerPosition.x) * radToDeg;
    DrawCircleSector(playerPosition, playerWeapon.range, click_angle - playerWeapon.spread,
                     click_angle + playerWeapon.spread, 2, RED);
    Vector2 endSegment1 = {
            playerPosition.x + playerWeapon.range * (float) cos((click_angle - playerWeapon.spread) * degToRad),
            playerPosition.y + playerWeapon.range * (float) sin((click_angle - playerWeapon.spread) * degToRad)};
    Vector2 endSegment2 = {
            playerPosition.x + playerWeapon.range * (float) cos((click_angle + playerWeapon.spread) * degToRad),
            playerPosition.y + playerWeapon.range * (float) sin((click_angle + playerWeapon.spread) * degToRad)};

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircleTriangle(position, radius.value, playerPosition,
                                         endSegment1, endSegment2, playerWeapon.range)) {
            health.value -= playerWeapon.damage;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * playerWeapon.pushback / m),
                        position.y + (position.y - playerPosition.y) * playerWeapon.pushback / m};
        }
    }
}

void playerSecondaryAttack(entt::registry &registry, entt::entity player) {
    Position playerPosition = registry.get<Position>(player);
    Weapon playerWeapon = registry.get<Weapon>(player);
    DrawCircle(playerPosition.x, playerPosition.y, playerWeapon.range, RED);

    auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircles(position, radius.value, playerPosition,
                                  playerWeapon.range)) {
            health.value -= playerWeapon.damage * 2;
            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            position = {(position.x + (position.x - playerPosition.x) * playerWeapon.pushback / m * 3),
                        position.y + (position.y - playerPosition.y) * playerWeapon.pushback / m * 3};
        }
    }
}

void castFire(entt::registry &registry, entt::entity player, Vector2 clickPosition) {}

void parseInput(entt::registry &registry, entt::entity player, Position &position, Camera2D &camera) {
    State playerState = registry.get<PlayerState>(player).state;
    if (IsKeyPressed(KEY_ONE)) {
        playerState = State::casting;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && playerState == State::normal) {
        playerAttack(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && playerState == State::casting) {
        castFire(registry, player, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        playerSecondaryAttack(registry, player);
    }


    position.y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
    position.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    position.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    position.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));

}


entt::entity spawnEnemy(entt::registry &registry, Position position) {
    entt::entity enemy = registry.create();
    registry.emplace<Enemy>(enemy);
    registry.emplace<Radius>(enemy, 15);
    registry.emplace<Living>(enemy);
    registry.emplace<Health>(enemy, 100, 100);
    registry.emplace<Position>(enemy, position.x, position.y);
    return enemy;
}

entt::entity spawnEnemy(entt::registry &registry) {
    return spawnEnemy(registry, Position{300.f, 300.f});
}


entt::entity spawnPlayer(entt::registry &registry) {
    entt::entity player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Living>(player);
    registry.emplace<Radius>(player, 20);
    registry.emplace<Weapon>(player, sword);
    registry.emplace<Health>(player, 100, 100);
    registry.emplace<Position>(player, 500.0f, 500.0f);
    registry.emplace<PlayerState>(player, State::normal);

    return player;
}

Camera2D spawnCamera() {
    Camera2D camera = {0};
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
}

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


int main() {
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 500);
    entt::registry registry;

    entt::entity player = spawnPlayer(registry);
    spawnEnemy(registry);
    Position &position = registry.get<Position>(player);

    InitWindow(screenWidth, screenHeight, "Apto Modo");
    SetTargetFPS(60);

    Camera2D camera = spawnCamera();
    Vector2 world{};
    unsigned int framesCounter = 0;
    while (!WindowShouldClose()) {
        world = GetScreenToWorld2D(Vector2{0, 0}, camera);
        parseInput(registry, player, position, camera);
        updateEnemy(registry, position);

        if (framesCounter % 60 == 0) {
            spawnEnemy(registry, {static_cast<float>(uniform_dist(e1)),
                                  static_cast<float>(uniform_dist(e1))});
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        draw(registry);

        DrawRectangleV(world, Vector2{80, 20}, {0, 0, 0, 100});
        DrawFPS((int) world.x, (int) world.y);
        EndDrawing();

        ++framesCounter;
    }

    CloseWindow();

    return 0;
}
