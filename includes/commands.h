//
// Created by ginko on 11/24/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include <bitset>
#include <managers/game.h>

class CommandBase {
public:
    virtual ~CommandBase() = default;

    virtual void execute() = 0;
};

struct CommandHolder {
    std::unique_ptr<CommandBase> command;
};

namespace Inputs {
    void Listen(entt::registry &registry,  const Camera2D& camera, float delta);

    void Update(entt::registry &registry);
}

namespace Command {
    class Move final : public CommandBase {
        entt::registry &registry;
        entt::entity self;
        std::bitset<4> bitfield;
        float delta{};

    public:
        Move(entt::registry &registry, entt::entity self, std::bitset<4> bitfield, float delta);

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
}
#endif //COMMANDS_H
