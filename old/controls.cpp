//
// Created by ginko on 02/03/24.
//

#include "pch.h"
#include "controls.h"

#include "external/miniaudio.h"
#include "math/mathConstants.h"

#include "collisions.h"
#include "constants.h"
#include "managers/audioManager.h"
#include "managers/animationManager.h"
#include "managers/configManager.h"
#include "attributes.h"
#include "items.h"


void playerAttack(entt::entity &player, Attributes &attributes, Vector2 clickPosition) {
    auto &attackTimer =Game::registry.get<AttackTimer>(player).timer;
    if (attackTimer.ElapsedSeconds() < attributes.getMultiplied(AttributeConstants::attackSpeed)) return;
    attackTimer.Reset();
    Position &playerPosition =Game::registry.get<Position>(player);

    float attackRange = attributes.getMultiplied(AttributeConstants::range);
    float attackSpread = attributes.getMultiplied(AttributeConstants::spread);
    float damage = attributes.getMultiplied(AttributeConstants::damagePhysical);
    //    float pushback =Game::registry.get<Pushback>(player);

    float clickAngle = atan2(clickPosition.y - playerPosition.y, clickPosition.x - playerPosition.x) * Math::Const::radToDeg;

    AttackEffect effect = {
        100, playerPosition, attackRange, clickAngle - attackSpread, clickAngle + attackSpread,
        PURPLE
    };
   Game::registry.emplace<AttackEffect>Game::registry.create(), effect);

   Game::registry.emplace<Audio::Command>Game::registry.create(), "player_shot");

    Vector2 endSegment1 = {
        playerPosition.x + attackRange *  cos((clickAngle - attackSpread) * Math::Const::degToRad),
        playerPosition.y + attackRange * sin((clickAngle - attackSpread) * Math::Const::degToRad)
    };
    const Vector2 endSegment2 = {
        playerPosition.x + attackRange * cos((clickAngle + attackSpread) * Math::Const::degToRad),
        playerPosition.y + attackRange *  sin((clickAngle + attackSpread) * Math::Const::degToRad)
    };

    auto enemyView =Game::registry.view<Enemy, Living, Health, Radius, Position>();
    for (auto [enemy, health, radius, position]: enemyView.each()) {
        if (CheckCollisionCircleTriangle(position, radius, playerPosition,
                                         endSegment1, endSegment2, attackRange)) {
            health -= damage;
            //            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
            //            position = {(position.x + (position.x - playerPosition.x) * pushback / m),
            //                        position.y + (position.y - playerPosition.y) * pushback / m};
        }
    }
}

void playerSecondaryAttack(entt::entity player) {
    //    Position playerPosition =Game::registry.get<Position>(player);
    //    float attackRange  =Game::registry.get<AttackRange>(player);
    //    float damage  =Game::registry.get<Damage>(player);
    //    float pushback =Game::registry.get<Pushback>(player);
    //    DrawCircle(playerPosition.x, playerPosition.y, attackRange, RED);
    //
    //    auto enemyView =Game::registry.view<Enemy, Living, Health, Radius, Position>();
    //    for (auto [enemy, health, radius, position]: enemyView.each()) {
    //        if (CheckCollisionCircles(position, radius, playerPosition,
    //                                  attackRange)) {
    //            health -= damage * 2;
    //            float m = sqrt(pow(playerPosition.x + position.x, 2) + pow(playerPosition.y + position.y, 2));
    //            position = {(position.x + (position.x - playerPosition.x) * pushback / m * 3),
    //                        position.y + (position.y - playerPosition.y) * pushback / m * 3};
    //        }
    //    }
}

void castFire(entt::entity player, Vector2 clickPosition) {
}

void PickUpItem(const entt::entity player) {
    Position playerPosition =Game::registry.get<Position>(player); // This could be static, or a static ref
    for (auto [entity, position]:Game::registry.view<Item, Position>().each()) {
        if (Vector2Distance(playerPosition, position) < 20) {
           Game::registry.emplace<OnPlayer>(entity);
           Game::registry.remove<Position>(entity);
            return; // Only draw it for one item
        }
    }
}

void selectEnemy(Vector2 worldPosition) {
    auto enemyView =Game::registry.view<Enemy, Living, Radius, Position>();
   Game::registry.clear<Selected>();
    for (auto [enemy, radius, position]: enemyView.each()) {
        if (CheckCollisionPointCircle(worldPosition, position, radius)) {
           Game::registry.emplace<Selected>(enemy);
            *Config::GetBoolPtr("show_enemy_window") = true;
        }
    }
}

Vector2 processMoveCommand(const Vector2 &position) {
    Vector2 futurePos = position;

    futurePos.y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
    futurePos.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    futurePos.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    futurePos.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));
    return futurePos;
}

void processDashCommand(Position &position, const LookAngle lookAngle, const Radius radius) {
    constexpr float dashDist = 60.0f;
    Vector2 futurePos = Vector2Add(position,
                                         Vector2Scale(Vector2Normalize(Vector2{
                                                          cos(lookAngle * DEG2RAD),
                                                          sin(lookAngle * DEG2RAD)
                                                      }),
                                                      dashDist));

    // check any obstacle in front, and clamp future pos to the intersection
    solveCircleRecCollision(futurePos, radius);
    position = futurePos;
}


void movePlayer(Position &position, Vector2 &futurePos, const float radius, const Attributes &attributes) {
    if (futurePos.x - radius < 0 || futurePos.x + radius > Const::mapWidth) futurePos.x = position.x;
    if (futurePos.y - radius < 0 || futurePos.y + radius > Const::mapHeight) futurePos.y = position.y;

    const Vector2 direction = Vector2Subtract(futurePos, position);
    const Vector2 movement = Vector2Scale(Vector2Normalize(direction),
                                          attributes.getMultiplied(AttributeConstants::speed));
    futurePos = Vector2Add(position, movement);
    solveCircleRecCollision(futurePos, radius);
    position = futurePos;
}


void parseInput(entt::entity &player, Position &position, Camera2D &camera) {
    auto &attributes =Game::registry.get<Attributes>(player);
    const auto radius =Game::registry.get<Radius>(player);
    const auto lookAngle =Game::registry.get<LookAngle>(player);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        playerAttack( player, attributes, GetScreenToWorld2D(GetMousePosition(), camera));
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        //        playerSecondaryAttack( player);
        selectEnemy( GetScreenToWorld2D(GetMousePosition(), camera));
    }

    if (IsKeyPressed(KEY_O)) *Config::GetBoolPtr("show_attr_window") = !Config::GetBool("show_attr_window");
    if (IsKeyPressed(KEY_M)) GetMasterVolume() == 0 ? SetMasterVolume(100) : SetMasterVolume(0);
    if (IsKeyPressed(KEY_I)) *Config::GetBoolPtr("show_inv_window") = !Config::GetBool("show_inv_window");


    if (IsKeyPressed(KEY_F)) {
        PickUpItem( player);
    }
    camera.zoom += GetMouseWheelMove() / 10;


    if (Config::GetBool("free_camera")) {
        camera.target.y -= 4.0f * static_cast<float>(IsKeyPressed(KEY_W) || IsKeyDown(KEY_W));
        camera.target.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
        camera.target.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
        camera.target.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));
    } else {
        Vector2 futurePos = processMoveCommand(position);
        if (IsKeyPressed(KEY_LEFT_SHIFT)) processDashCommand(position, lookAngle, radius);
        movePlayer(position, futurePos, radius, attributes);
    }
}


void updateAttributes(const Attributes &attributes) {
    std::vector<AttributeConstants::Modifier> modifiers{};
    auto modifiersView =Game::registry.view<OnPlayer, AttributeConstants::Modifier>();
    for (auto [entity, modifier]: modifiersView.each()) {
        modifiers.emplace_back(modifier);
    }
    attributes.updateModifiers(modifiers);
}

void faceMouse(const Vector2 &position, LookAngle &lookAngle, const Camera2D &camera) {
    const auto [mouseX, mouseY] = GetScreenToWorld2D(GetMousePosition(), camera);
    lookAngle = atan2(mouseY - position.y, mouseX - position.x) * RAD2DEG;
}

void updatePlayer(entt::entity &player, Position &position, Camera2D &camera) {
    parseInput( player, position, camera);
    auto &lookAngle =Game::registry.get<LookAngle>(player);
    auto &attributes =Game::registry.get<Attributes>(player);
    auto health =Game::registry.get<Health>(player);
    const auto exp =Game::registry.get<Experience>(player);

    faceMouse(position, lookAngle, camera);
    if (health.max < health.value) health.value = health.max; // TODO fix this
    if (exp >= attributes.expToNextLevel()) attributes.levelUp();
    updateAttributes( attributes);
}
