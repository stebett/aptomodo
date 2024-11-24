//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <collisions.h>
#include <components.h>
#include <constants.h>


void commandSystem(entt::registry &registry) {
    auto view = registry.view<CommandHolder>();
    // todo the view should clean itself each time? or only if commands are successfully executed?
    // maybe commands can reinsert themselves if they fail
    for (auto [e, holder]: view.each()) {
        holder.command->execute();
    }
}


// void generateCommands(entt::registry &registry) {
//     auto player = registry.view<Player>().front();
//     if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) {
//         registry.emplace_or_replace<CommandHolder>(player,
//                                                    std::make_unique<MoveCommand>(registry, player, Direction::UP));
//     }
// }


MoveCommand::MoveCommand(entt::registry &registry, const entt::entity self, const Direction direction)
: registry(registry), direction(direction), self(self) {
}

void MoveCommand::execute() {
    Position& position = registry.get<Position>(self);
    const Radius& radius = registry.get<Radius>(self);
    const Attributes& attributes = registry.get<Attributes>(self);
    Vector2 futurePos = registry.get<Position>(self);

    futurePos.y -= 4.0f * static_cast<float>(direction == Direction::UP);
    // futurePos.y += 4.0f * static_cast<float>(IsKeyPressed(KEY_S) || IsKeyDown(KEY_S));
    // futurePos.x -= 4.0f * static_cast<float>(IsKeyPressed(KEY_A) || IsKeyDown(KEY_A));
    // futurePos.x += 4.0f * static_cast<float>(IsKeyPressed(KEY_D) || IsKeyDown(KEY_D));

    if (futurePos.x - radius < 0 || futurePos.x + radius > mapWidth) futurePos.x = position.x;
    if (futurePos.y - radius < 0 || futurePos.y + radius > mapHeight) futurePos.y = position.y;

    const Vector2 direction = Vector2Subtract(futurePos, position);
    const Vector2 movement = Vector2Scale(Vector2Normalize(direction),
                                          attributes.getMultiplied(AttributeConstants::speed));
    futurePos = Vector2Add(position, movement);
    solveCircleRecCollision(futurePos, radius);
    position = futurePos;
}
