//
// Created by ginko on 11/29/24.
//

#ifndef INPUTS_H
#define INPUTS_H

#include <camera.h>
#include "commands.h"


using DelayedCommandVariant = std::variant<
        Delayed<void (*)(entt::entity, std::bitset<4>), entt::entity, std::bitset<4>>,
        Delayed<void (*)(entt::entity), entt::entity>,
        Delayed<void (*)(GameCamera &, const std::bitset<4>, const float), GameCamera, std::bitset<4>, float>,
        Delayed<void (*)(entt::entity, Vector2), entt::entity, Vector2>,
        Delayed<void (*)(const Vector2 &), Vector2>,
        Delayed<void (*)()>
>;


namespace Inputs {
    std::vector<DelayedCommandVariant> Listen(GameCamera &camera, float delta);

    void Update(std::vector<DelayedCommandVariant>);
}


#endif //INPUTS_H
