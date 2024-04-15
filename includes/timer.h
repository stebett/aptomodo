//
// Created by ginko on 15/04/24.
//

#ifndef ACEROLA_JAM0_TIMER_H
#define ACEROLA_JAM0_TIMER_H

#include <chrono>


class Timer {
public:
    Timer() {
        Reset();
    }

    void Reset() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    [[nodiscard]] float Elapsed() const  {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
    }

    [[nodiscard]] float ElapsedMillis() const {
        return Elapsed() * 1000.0f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

#endif //ACEROLA_JAM0_TIMER_H
