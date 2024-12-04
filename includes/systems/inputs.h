//
// Created by ginko on 11/29/24.
//

#ifndef INPUTS_H
#define INPUTS_H

#include <camera.h>
#include "commands.h"

namespace Inputs {
    std::vector<DelayedCommandVariant> Listen(entt::registry &registry, GameCamera &camera, float delta);

    void Update(std::vector<DelayedCommandVariant>);
}



#endif //INPUTS_H
