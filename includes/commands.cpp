//
// Created by ginko on 11/24/24.
//

#include "commands.h"

#include <attributes.h>
#include <bitset>
#include <collisions.h>
#include <components.h>
#include <constants.h>

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


    void Listen(entt::registry &registry, float delta) {
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
    }
}

namespace Command {
    Move::Move(entt::registry &registry, const entt::entity self, const std::bitset<4> bitfield, const float delta)
        : registry(registry), self(self), bitfield(bitfield), delta(delta){
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
        constexpr float deltaCorrector {60};
        const Vector2 movement = Vector2Scale(Vector2Normalize(direction), attributes.getMultiplied(speed)*delta*deltaCorrector);
        // TODO Speed should definitely be just a component, that gets updated any time attributes change
        futurePos = Vector2Add(position, movement);
        solveCircleRecCollision(futurePos, radius);
        // TODO add solve npc collisions
        position = futurePos;
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
};
