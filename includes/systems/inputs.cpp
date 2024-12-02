//
// Created by ginko on 11/29/24.
//

#include "inputs.h"

#include <commands.h>
#include <components.h>
#include "managers/configManager.h"
#include <managers/game.h>
#include <managers/gui.h>


namespace Inputs {
    void Update(entt::registry &registry) {
        registry.view<CommandHolder>(entt::exclude<ExecuteLate>).each([](auto entity, const CommandHolder &holder) {
            holder.command->execute();
        });
        registry.view<CommandHolder, ExecuteLate>().each([](auto entity, const CommandHolder &holder) {
            holder.command->execute();
        });
        registry.clear<CommandHolder>();
    }


    void Listen(entt::registry &registry, GameCamera &camera, float delta) {
        auto player = registry.view<Player>().front();
        if (!Gui::WantKeyboard() || !Gui::WantMouse()) {
            std::bitset<4> bitfield;
            if (!Game::IsPaused()) {
                if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) bitfield.set(0);
                if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) bitfield.set(1);
                if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) bitfield.set(2);
                if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) bitfield.set(3);
                if (!bitfield.none())
                    if (Config::GetBool("free_camera")) {
                        registry.emplace_or_replace<CommandHolder>(
                            entt::entity(), std::make_unique<Command::MoveCamera>(camera, bitfield, delta));
                    } else {
                        registry.emplace_or_replace<CommandHolder>(
                            entt::entity(), std::make_unique<Command::Move>(registry, player, bitfield));
                    }
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
            if (IsKeyPressed(KEY_LEFT_SHIFT))
                registry.emplace_or_replace<CommandHolder>(
                    registry.create(), std::make_unique<Command::Dash>(registry, player));
        }
        if (!Gui::WantMouse() && !Config::GetBool("in_editor")) {
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
        }
    }
}
