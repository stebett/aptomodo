//
// Created by ginko on 11/29/24.
//

#ifndef STATUS_H
#define STATUS_H
#include <timer.h>


namespace StatusEffect {
    struct Dash {
        PassiveTimer timer;

        explicit Dash(float duration);
    };
}


#endif //STATUS_H
