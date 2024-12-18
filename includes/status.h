//
// Created by ginko on 11/29/24.
//

#ifndef STATUS_H
#define STATUS_H

#include <timer.h>

// TODO this file can be renamed to statusEffect and it can go in a different dir (probably with commands and components and factories?)

namespace StatusEffect {
    struct Dash {
        PassiveTimer timer;

        explicit Dash(float duration);
    };
}


#endif //STATUS_H
