//
// Created by ginko on 11/29/24.
//

#include "status.h"

namespace StatusEffect {
    Dash::Dash(const std::chrono::duration<long double> duration): duration(duration) {
        start = std::chrono::high_resolution_clock::now();
    }
}
