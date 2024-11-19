//
// Created by ginko on 11/19/24.
//

#include "decay.h"

float expDecay(const float a, const float b, const float decay, const float dt) {
    return b+(a-b)*exp(-decay*dt);
}
