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

#include "systems/attacks.h"
#include "math/mathConstants.h"
#include "status.h"
#include <bitset>


namespace Command {

    void ToggleMenu() {
        *Config::GetBoolPtr("show_menu") = !*Config::GetBoolPtr("show_menu");
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
        const auto playerBody = Game::registry.get<b2BodyId>(self);
        std::string attackName = Config::GetString("player_attack_name");
        Attacks::Assign(playerBody, attackName);
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

    void LoadTestShader() {
        UnloadShader(Game::testShader);
        Game::testShader = LoadShader(nullptr, (std::filesystem::path(ROOT_PATH) / "assets/shaders/test.fs").c_str());
        if (Game::testShader.id == 0) {
            Game::testShader = LoadShader(nullptr, (std::filesystem::path(ROOT_PATH) / "assets/shaders/base.fs").c_str());

        }

    }


}
