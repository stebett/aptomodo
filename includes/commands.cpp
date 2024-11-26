//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <bitset>
#include <collisions.h>
#include <components.h>
#include <constants.h>


void commandSystem(entt::registry &registry) {
    auto view = registry.view<CommandHolder>();
    // todo the view should clean itself each time? or only if commands are successfully executed?
    // maybe commands can reinsert themselves if they fail
    for (auto [e, holder]: view.each()) {
        holder.command->execute();
        registry.remove<CommandHolder>(e);
    }
}


void generateCommands(entt::registry &registry) {
    auto player = registry.view<Player>().front();
    std::bitset<4> bitfield;
    if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) bitfield.set(0);
    if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) bitfield.set(1);
    if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) bitfield.set(2);
    if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) bitfield.set(3);
    if (!bitfield.none()) registry.emplace_or_replace<CommandHolder>(player, std::make_unique<MoveCommand>(registry, player, bitfield));
}


MoveCommand::MoveCommand(entt::registry &registry, const entt::entity self, const std::bitset<4> bitfield)
    : registry(registry), bitfield(bitfield), self(self) {
}

void MoveCommand::execute() { // TODO make this deltaTime dependant
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
    const Vector2 movement = Vector2Scale(Vector2Normalize(direction), attributes.getMultiplied(speed));
    // TODO Speed should definitely be just a component, that gets updated any time attributes change
    futurePos = Vector2Add(position, movement);
    solveCircleRecCollision(futurePos, radius);
    // TODO add solve npc collisions
    position = futurePos;
}
