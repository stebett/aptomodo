//
// Created by ginko on 11/24/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

struct CommandHolder {
    std::unique_ptr<Command> command;
};

enum class Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    UPRIGHT,
    UPLEFT,
    DOWNRIGHT,
    DOWNLEFT,
    RIGHTUP,
    RIGHTDOWN,
    LEFTUP,
    LEFTDOWN,
    NONE
};

class MoveCommand final : Command {


    entt::registry& registry;
    entt::entity self;
    Direction direction;
public:
    MoveCommand(entt::registry &registry, entt::entity self, Direction direction);
    void execute() override;
};

void generateCommands(entt::registry &registry);


#endif //COMMANDS_H
