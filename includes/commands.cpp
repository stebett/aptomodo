//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <components.h>
#include "managers/configManager.h"
#include <items.h>
#include <managers/game.h>
#include <managers/gui.h>
#include <systems/physics.h>

#include "attacks.h"
#include "math/mathConstants.h"
#include "status.h"
#include <bitset>


namespace Command {

    void ToggleMenu() {
        *Config::GetBoolPtr("show_menu") = !Config::GetBoolPtr("show_menu");
    }

    void Move(entt::entity self, const std::bitset<4> bitfield) {
        const auto &attributes = Game::registry.get<Attributes>(self);
        const auto body = Game::registry.get<b2BodyId>(self);
        Vector2 direction = {0.0f, 0.0f};

        direction.y -= static_cast<float>(bitfield[0]);
        direction.y += static_cast<float>(bitfield[1]);
        direction.x -= static_cast<float>(bitfield[2]);
        direction.x += static_cast<float>(bitfield[3]);

        constexpr int framerateCorrection = 60;
        const bool isDashing = Game::registry.all_of<StatusEffect::Dash>(self);
        const auto dashSpeed = Config::GetFloat("dash_speed");
        const float actualSpeed = isDashing ? dashSpeed : framerateCorrection * attributes.getMultiplied(speed);
        const Vector2 velocity = Vector2Scale(Vector2Normalize(direction), actualSpeed);
        b2Body_SetLinearVelocity(body, {velocity.x, velocity.y});
    }

    void Dash(entt::entity self) {
        const float dashDuration = Config::GetFloat("dash_duration");
        Game::registry.emplace_or_replace<StatusEffect::Dash>(self, dashDuration);
    }

    void MoveCamera(GameCamera &camera, const std::bitset<4> bitfield, const float delta) {
        Vector2 movement = {0, 0};
        movement.y -= 4.0f * static_cast<float>(bitfield[0]);
        movement.y += 4.0f * static_cast<float>(bitfield[1]);
        movement.x -= 4.0f * static_cast<float>(bitfield[2]);
        movement.x += 4.0f * static_cast<float>(bitfield[3]);
        camera.MoveFreeCamera(movement);
    }

    void Attack(entt::entity self, const Vector2 mousePosition) {
        auto &attackTimer = Game::registry.get<AttackTimer>(self).timer;
        const auto &attributes = Game::registry.get<Attributes>(self);
        const auto lookAngle = Game::registry.get<LookAngle>(self);
        const auto body = Game::registry.get<b2BodyId>(self);
        const auto radius = Game::registry.get<Radius>(self);
        if (attackTimer.ElapsedSeconds() < attributes.getMultiplied(AttributeConstants::attackSpeed)) return;
        attackTimer.Reset();
        Position &playerPosition = Game::registry.get<Position>(self);

        auto swordEntity = Game::registry.create();
        constexpr float halfWidth = 3;
        constexpr float halfHeight = 25;
        b2BodyId swordBodyId = Physics::EmplaceSword(halfWidth, halfHeight);
        Physics::ConnectBodyToEntity(swordBodyId, swordEntity);
        Game::registry.emplace<Attacks::Attack>(swordEntity, Attacks::Attack{100.0f});
        b2Transform front = b2Transform({halfHeight + radius, 0}, b2Rot(0, 1));
        b2Transform beg = b2Transform({halfHeight + 6, 6}, b2Rot(cos(-0.8), sin(-0.8)));
        b2Transform end = b2Transform({halfHeight + radius, 0}, b2Rot(cos(0), sin(0)));
        Game::registry.emplace<Attacks::LocalTransform>(swordEntity, beg, front);
        Game::registry.emplace<Attacks::BodyCouple>(swordEntity, Attacks::BodyCouple{body, swordBodyId});
        Game::registry.emplace<PassiveTimer>(swordEntity, 0.5f);

        const float attackRange = attributes.getMultiplied(AttributeConstants::range);
        const float attackSpread = attributes.getMultiplied(AttributeConstants::spread);
        const float damage = attributes.getMultiplied(AttributeConstants::damagePhysical);

        float clickAngle = atan2(mousePosition.y - playerPosition.y, mousePosition.x - playerPosition.x) *
                           Math::Const::radToDeg;

    }

    void PickUp(entt::entity self) {
        const Position playerPosition = Game::registry.get<Position>(self);
        for (auto [entity, position]: Game::registry.view<Item, Position>().each()) {
            if (Vector2Distance(playerPosition, position) < 20) {
                Game::registry.emplace<OnPlayer>(entity);
                Game::registry.remove<Position>(entity);
                return; // Only draw it for one item
            }
        }
    }

    void Quit() {
        Game::SetOutcome(LevelOutcome::QUIT);
        Game::ExitLevel();
    }

    void Restart() {
        Game::SetOutcome(LevelOutcome::RESTART);
        Game::ExitLevel();
    }

    void Pause() {
        Game::Pause();
    }

    void SelectEnemy(const Vector2 &mouse_position) {
        const auto enemyView = Game::registry.view<Enemy, ToRender, Living, Radius, Position>();
        Game::registry.clear<Selected>();
        for (auto [enemy, radius, position]: enemyView.each()) {
            if (CheckCollisionPointCircle(mouse_position, position, radius)) {
                Game::registry.emplace<Selected>(enemy);
                *Config::GetBoolPtr("show_enemy_window") = true;
            }
        }
    }

    void Mute() {
        GetMasterVolume() == 0 ? SetMasterVolume(100) : SetMasterVolume(0);
    }

}
