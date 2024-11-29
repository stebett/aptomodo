//
// Created by ginko on 11/29/24.
//

#ifndef INPUTS_H
#define INPUTS_H

#include <camera.h>

namespace Inputs {
    void Listen(entt::registry &registry, GameCamera &camera, float delta);

    void Update(entt::registry &registry);
}



#endif //INPUTS_H
