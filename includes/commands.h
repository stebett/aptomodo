//
// Created by ginko on 11/24/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H

#include <camera.h>
#include <type_traits>


template<typename Callable, typename... Args>
concept InvocableWith = requires(Callable c, Args... args) {
    std::invoke(c, args...); // Ensures the callable can be invoked with the arguments
};


template<typename Callable, typename... Args> requires InvocableWith<Callable, Args...>
class Delayed {
    Callable callable;
    std::tuple<Args...> args;

public:
    explicit Delayed(Callable callable, Args... args)
            : callable(callable), args(std::make_tuple(std::forward<Args>(args)...)) {}

    void operator()() {
        std::apply([this](auto &&... function_args) {
            std::invoke(callable, std::forward<decltype(function_args)>(function_args)...);
        }, args);
    }
};

namespace Command {

    void Move(entt::entity self, std::bitset<4> bitfield);

    void Dash(entt::entity self);

    void MoveCamera(GameCamera &camera, std::bitset<4> bitfield, float delta);

    void Attack(entt::entity self, Vector2 mousePosition);

    void PickUp(entt::entity self);

    void Quit();

    void Restart();

    void Pause();

    void SelectEnemy(const Vector2 &mouse_position);

    void Mute();

    void ToggleMenu();
}

#endif //COMMANDS_H
