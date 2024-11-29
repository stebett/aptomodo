//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <collisions.h>
#include <components.h>
#include <config.h>
#include <items.h>
#include <managers/audioManager.h>
#include <managers/game.h>
#include <managers/gui.h>

#include "math/mathConstants.h"
#include "status.h"
#include "status.h"


namespace Command {
    Move::Move(entt::registry &registry, const entt::entity self, const std::bitset<4> bitfield)
        : registry(registry), self(self), bitfield(bitfield) {
    }

    void Move::execute() {
        const auto &attributes = registry.get<Attributes>(self);
        const auto body = registry.get<b2BodyId>(self);
        Vector2 direction = {0.0f, 0.0f};

        direction.y -= static_cast<float>(bitfield[0]);
        direction.y += static_cast<float>(bitfield[1]);
        direction.x -= static_cast<float>(bitfield[2]);
        direction.x += static_cast<float>(bitfield[3]);

        constexpr int framerateCorrection = 60;
        const bool isDashing = registry.all_of<StatusEffect::Dash>(self);
        const auto dashSpeed = Config::GetFloat("dash_speed");
        const float actualSpeed = isDashing ? dashSpeed : framerateCorrection * attributes.getMultiplied(speed);
        const Vector2 velocity = Vector2Scale(Vector2Normalize(direction), actualSpeed);
        b2Body_SetLinearVelocity(body, {velocity.x, velocity.y});
    }

    Dash::Dash(entt::registry &registry, const entt::entity self)
        : registry(registry), self(self) {
    }

    void Dash::execute() {
        const float dashDuration = Config::GetFloat("dash_duration");
        auto duration_in_seconds = std::chrono::duration<float>(dashDuration);
        registry.emplace_or_replace<StatusEffect::Dash>(self, duration_in_seconds);
    }

    MoveCamera::MoveCamera(GameCamera &camera, const std::bitset<4> bitfield, const float delta)
        : camera(camera), bitfield(bitfield), delta(delta) {
    }

    void MoveCamera::execute() {
        Vector2 movement = {0, 0};
        movement.y -= 4.0f * static_cast<float>(bitfield[0]);
        movement.y += 4.0f * static_cast<float>(bitfield[1]);
        movement.x -= 4.0f * static_cast<float>(bitfield[2]);
        movement.x += 4.0f * static_cast<float>(bitfield[3]);
        camera.MoveFreeCamera(movement);
    }


    Attack::Attack(entt::registry &registry, entt::entity self, const Vector2 mousePosition)
        : registry(registry), self(self), mousePosition(mousePosition) {
    }

    void Attack::execute() {
        auto &attackTimer = registry.get<AttackTimer>(self).timer;
        const auto &attributes = registry.get<Attributes>(self);
        if (attackTimer.ElapsedSeconds() < attributes.getMultiplied(AttributeConstants::attackSpeed)) return;
        attackTimer.Reset();
        Position &playerPosition = registry.get<Position>(self);

        const float attackRange = attributes.getMultiplied(AttributeConstants::range);
        const float attackSpread = attributes.getMultiplied(AttributeConstants::spread);
        const float damage = attributes.getMultiplied(AttributeConstants::damagePhysical);
        //    float pushback = registry.get<Pushback>(player);

        float clickAngle = atan2(mousePosition.y - playerPosition.y, mousePosition.x - playerPosition.x) *
                           Math::Const::radToDeg;

        AttackEffect effect = {
            100, playerPosition, attackRange, clickAngle - attackSpread, clickAngle + attackSpread,
            PURPLE
        };
        registry.emplace<AttackEffect>(registry.create(), effect);

        registry.emplace<Audio::Command>(registry.create(), "player_shot");

        Vector2 endSegment1 = {
            playerPosition.x + attackRange * cos((clickAngle - attackSpread) * Math::Const::radToDeg),
            playerPosition.y + attackRange * sin((clickAngle - attackSpread) * Math::Const::radToDeg)
        };
        Vector2 endSegment2 = {
            playerPosition.x + attackRange * cos((clickAngle + attackSpread) * Math::Const::radToDeg),
            playerPosition.y + attackRange * sin((clickAngle + attackSpread) * Math::Const::radToDeg)
        };

        auto enemyView = registry.view<Enemy, Living, Health, Radius, Position>();
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

    PickUp::PickUp(entt::registry &registry, entt::entity self): registry(registry),
                                                                 self(self) {
    }

    void PickUp::execute() {
        const Position playerPosition = registry.get<Position>(self);
        for (auto [entity, position]: registry.view<Item, Position>().each()) {
            if (Vector2Distance(playerPosition, position) < 20) {
                registry.emplace<OnPlayer>(entity);
                registry.remove<Position>(entity);
                return; // Only draw it for one item
            }
        }
    }

    void Quit::execute() {
        Game::SetOutcome(LevelOutcome::QUIT);
        Game::ExitLevel();
    }

    void Restart::execute() {
        Game::SetOutcome(LevelOutcome::RESTART);
        Game::ExitLevel();
    }

    void Pause::execute() {
        Game::Pause();
    }

    SelectEnemy::SelectEnemy(entt::registry &registry, const Vector2 &mouse_position)
        : registry(registry),
          mousePosition(mouse_position) {
    }

    void SelectEnemy::execute() {
        const auto enemyView = registry.view<Enemy, ToRender, Living, Radius, Position>();
        registry.clear<Selected>();
        for (auto [enemy, radius, position]: enemyView.each()) {
            if (CheckCollisionPointCircle(mousePosition, position, radius)) {
                registry.emplace<Selected>(enemy);
                *Config::GetBoolPtr("show_enemy_window") = true;
            }
        }
    }

    void Mute::execute() {
        GetMasterVolume() == 0 ? SetMasterVolume(100) : SetMasterVolume(0);
    }
};
