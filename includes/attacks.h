//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H


namespace Attacks {
    struct Hit {
    };

    class Sword {
    public:
        float damage = 0;

        explicit Sword(const float damage) : damage(damage) {
        };
    };
}


#endif //ATTACKS_H
