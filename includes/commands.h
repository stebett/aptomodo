//
// Created by ginko on 11/24/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include <camera.h>


class CommandBase {
public:
    virtual ~CommandBase() = default;

    virtual void execute() = 0;
};


struct ExecuteLate {};
struct CommandHolder {
    std::unique_ptr<CommandBase> command;
};



namespace Command {
    class Move final : public CommandBase {
        entt::registry &registry;
        entt::entity self;
        std::bitset<4> bitfield;

    public:
        Move(entt::registry &registry, entt::entity self, std::bitset<4> bitfield);

        void execute() override;
    };

    class Dash final : public CommandBase {
        entt::registry &registry;
        const entt::entity self;

    public:
        Dash(entt::registry &registry, entt::entity self);

        void execute() override;
    };

    class MoveCamera final : public CommandBase {
        GameCamera &camera;
        std::bitset<4> bitfield;
        float delta{};

    public:
        MoveCamera(GameCamera &camera, std::bitset<4> bitfield, float delta);

        void execute() override;
    };

    class Attack final : public CommandBase {
        entt::registry &registry;
        entt::entity self;
        Vector2 mousePosition;

    public:
        Attack(entt::registry &registry, entt::entity self, Vector2 mousePosition);

        void execute() override;
    };

    class PickUp final : public CommandBase {
        entt::registry &registry;
        entt::entity self;

    public:
        PickUp(entt::registry &registry, entt::entity self);

        void execute() override;
    };

    class Quit final : public CommandBase {
    public:
        void execute() override;
    };

    class Restart final : public CommandBase {
    public:
        void execute() override;
    };

    class Pause final : public CommandBase {
    public:
        void execute() override;
    };

    class SelectEnemy final : public CommandBase {
        entt::registry &registry;
        Vector2 mousePosition;

    public:
        SelectEnemy(entt::registry &registry, const Vector2 &mouse_position);

        void execute() override;
    };

    class Mute final : public CommandBase {
    public:
        void execute() override;
    };
}
#endif //COMMANDS_H
