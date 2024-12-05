//
// Created by ginko on 11/29/24.
//

#include "inputs.h"

#include <commands.h>
#include <components.h>
#include "managers/configManager.h"
#include <managers/game.h>
#include <managers/gui.h>
#include <functional>

namespace Inputs {
    void Update(std::vector<DelayedCommandVariant> commands) {
        for (auto &command: commands) {
            std::visit([](auto &cmd) { cmd(); }, command);
        }
    }


    std::vector<DelayedCommandVariant> Listen(GameCamera &camera, float delta) {
        std::vector<DelayedCommandVariant> commands{};
        auto pairKeyPress = [&commands](int key, auto functor) {
            if (IsKeyPressed(key))
                commands.emplace_back(std::forward<decltype(functor)>(functor));
        };
        auto pairMouseKeyPress = [&commands](int key, auto functor) {
            if (IsMouseButtonPressed(key))
                commands.emplace_back(std::forward<decltype(functor)>(functor));
        };
        auto player = Game::registry.view<Player>().front();
        if (!Gui::WantKeyboard() || !Gui::WantMouse()) {
            std::bitset<4> bitfield;
            if (!Game::IsPaused()) {
                if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) bitfield.set(0);
                if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) bitfield.set(1);
                if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) bitfield.set(2);
                if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) bitfield.set(3);
                if (!bitfield.none()) {
                    if (Config::GetBool("free_camera")) {
                        commands.emplace_back(Delayed(Command::MoveCamera, camera, bitfield, delta));
                    } else {
                        commands.emplace_back(Delayed(Command::Move, player, bitfield));
                    }
                }
            }
        }

        pairKeyPress(KEY_Q, Delayed(Command::Quit));
        pairKeyPress(KEY_Q, Delayed(Command::Quit));
        pairKeyPress(KEY_R, Delayed(Command::Restart));
        pairKeyPress(KEY_P, Delayed(Command::Pause));
        pairKeyPress(KEY_M, Delayed(Command::Mute));
        pairKeyPress(KEY_E, Delayed(Command::PickUp, player));
        pairKeyPress(KEY_LEFT_SHIFT, Delayed(Command::Dash, player));

        if (!Gui::WantMouse() && !Config::GetBool("in_editor")) {
            pairMouseKeyPress(MOUSE_BUTTON_RIGHT,
                              Delayed(Command::SelectEnemy, GetScreenToWorld2D(GetMousePosition(), camera)));
            pairMouseKeyPress(MOUSE_BUTTON_LEFT,
                              Delayed(Command::Attack, player, GetScreenToWorld2D(GetMousePosition(), camera)));
        }
        return commands;
    }
}