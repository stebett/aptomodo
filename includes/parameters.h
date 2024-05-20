//
// Created by ginko on 20/05/24.
//

#ifndef APTOMODO_PARAMETERS_H
#define APTOMODO_PARAMETERS_H

#include <array>

namespace parameters {
    extern int fps;
    extern int strategy;
    extern int enemy_walking_animation_fps;
    extern std::array<float, 18> subAttrMultipliers;
    extern std::array<int, 18> subAttrAtStart;
}
#endif //APTOMODO_PARAMETERS_H
