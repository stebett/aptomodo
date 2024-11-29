//
// Created by ginko on 11/29/24.
//

#ifndef STATUS_H
#define STATUS_H


namespace StatusEffect {
    struct Dash {
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::duration<long double> duration;

        explicit Dash(std::chrono::duration<long double> duration);
    };
}


#endif //STATUS_H
