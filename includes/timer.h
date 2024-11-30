//
// Created by ginko on 15/04/24.
//

#ifndef APTOMODO_TIMER_H
#define APTOMODO_TIMER_H

#include <chrono>

struct PassiveTimer {
    double start;
    double duration;

    explicit PassiveTimer(const double durationSeconds)
    : duration(durationSeconds) {
        start = GetTime();
    }
};


class Timer {
public:
    Timer() {
        Reset();
    }

    void Reset() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    void StartBehind(const int seconds) {
        m_Start = std::chrono::high_resolution_clock::now() - std::chrono::seconds(seconds);
    }

    [[nodiscard]] float ElapsedSeconds() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
    }

    [[nodiscard]] float ElapsedMillis() const {
        return ElapsedSeconds() * 1000.0f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

#endif //APTOMODO_TIMER_H
