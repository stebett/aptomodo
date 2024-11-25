//
// Created by ginko on 11/24/24.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#include <bitset>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

struct CommandHolder {
    std::unique_ptr<Command> command;
};

void generateCommands(entt::registry &registry);

void commandSystem(entt::registry &registry);

class MoveCommand final : public Command {
    entt::registry &registry;
    entt::entity self;
    std::bitset<4> bitfield;

public:
    MoveCommand(entt::registry &registry, entt::entity self, std::bitset<4> bitfield);

    void execute() override;
};





#endif //COMMANDS_H
