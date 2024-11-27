//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <bitset>
#include <collisions.h>
#include <components.h>
#include <config.h>
#include <constants.h>
#include <items.h>
#include <managers/audioManager.h>

namespace Inputs {
    void Update(entt::registry &registry) {
        auto view = registry.view<CommandHolder>();
        // todo the view should clean itself each time? or only if commands are successfully executed?
        // maybe commands can reinsert themselves if they fail
        for (auto [e, holder]: view.each()) {
            holder.command->execute();
            registry.remove<CommandHolder>(e);
        }
    }


    void Listen(entt::registry &registry, Camera2D &camera, float delta) {
        auto player = registry.view<Player>().front();
        std::bitset<4> bitfield;
        if (!Game::IsPaused()) {
            if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) bitfield.set(0);
            if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) bitfield.set(1);
            if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) bitfield.set(2);
            if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) bitfield.set(3);
            if (!bitfield.none())
                registry.emplace_or_replace<CommandHolder>(
                    entt::entity(), std::make_unique<Command::Move>(registry, player, bitfield, delta));
        }
        if (IsKeyPressed(KEY_Q))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(), std::make_unique<Command::Quit>());
        if (IsKeyPressed(KEY_R))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(), std::make_unique<Command::Restart>());
        if (IsKeyPressed(KEY_P))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(), std::make_unique<Command::Pause>());
        if (IsKeyPressed(KEY_M))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(), std::make_unique<Command::Mute>());
        if (IsKeyPressed(KEY_P))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(), std::make_unique<Command::PickUp>(registry, player));

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(),
                std::make_unique<Command::SelectEnemy>(registry,
                                                       GetScreenToWorld2D(GetMousePosition(), camera)));

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(),
                std::make_unique<Command::Attack>(registry,
                                                  player,
                                                  GetScreenToWorld2D(GetMousePosition(), camera)));

        if (abs(GetMouseWheelMove()) > 0.1)
            registry.emplace_or_replace<CommandHolder>(
                entt::entity(),
                std::make_unique<Command::Zoom>(camera));
    }
}

namespace Command {
    Move::Move(entt::registry &registry, const entt::entity self, const std::bitset<4> bitfield, const float delta)
        : registry(registry), self(self), bitfield(bitfield), delta(delta) {
    }

    void Move::execute() {
        // TODO make this deltaTime dependant
        Position &position = registry.get<Position>(self);
        const Radius &radius = registry.get<Radius>(self);
        const Attributes &attributes = registry.get<Attributes>(self);
        Vector2 futurePos = registry.get<Position>(self);

        futurePos.y -= 4.0f * static_cast<float>(bitfield[0]);
        futurePos.y += 4.0f * static_cast<float>(bitfield[1]);
        futurePos.x -= 4.0f * static_cast<float>(bitfield[2]);
        futurePos.x += 4.0f * static_cast<float>(bitfield[3]);

        if (futurePos.x - radius < 0 || futurePos.x + radius > mapWidth) futurePos.x = position.x;
        if (futurePos.y - radius < 0 || futurePos.y + radius > mapHeight) futurePos.y = position.y;

        const Vector2 direction = Vector2Subtract(futurePos, position);
        constexpr float deltaCorrector{60};
        const Vector2 movement = Vector2Scale(Vector2Normalize(direction),
                                              attributes.getMultiplied(speed) * delta * deltaCorrector);
        // TODO Speed should definitely be just a component, that gets updated any time attributes change
        futurePos = Vector2Add(position, movement);
        solveCircleRecCollision(futurePos, radius);
        // TODO add solve npc collisions
        position = futurePos;
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

        float clickAngle = atan2(mousePosition.y - playerPosition.y, mousePosition.x - playerPosition.x) * radToDeg;

        AttackEffect effect = {
            100, playerPosition, attackRange, clickAngle - attackSpread, clickAngle + attackSpread,
            PURPLE
        };
        registry.emplace<AttackEffect>(registry.create(), effect);

        registry.emplace<Audio::Command>(registry.create(), "player_shot");

        Vector2 endSegment1 = {
            playerPosition.x + attackRange * (float) cos((clickAngle - attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((clickAngle - attackSpread) * degToRad)
        };
        Vector2 endSegment2 = {
            playerPosition.x + attackRange * (float) cos((clickAngle + attackSpread) * degToRad),
            playerPosition.y + attackRange * (float) sin((clickAngle + attackSpread) * degToRad)
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

    Zoom::Zoom(Camera2D &camera)
        : camera(camera) {
    }

    void Zoom::execute() {
        camera.zoom += GetMouseWheelMove() / 10;
    }
};
