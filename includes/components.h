//
// Created by ginko on 01/03/24.
//

#ifndef APTOMODO_COMPONENTS_H
#define APTOMODO_COMPONENTS_H

#include "timer.h"

// Tags
struct Player {
};

struct Enemy {
};

struct Living {
};

struct ToRender {
};

struct ToSimulate {
};

struct Chasing {
    Timer timer;
    float duration;

    explicit Chasing(float p_duration = 3.0f);

    [[nodiscard]] bool isChasing() const;
};

struct Selected {
};

struct ColorBB {
    Color color;

    operator Color() const noexcept;
};

// Components
struct Position {
    float x;
    float y;

    operator Vector2() const noexcept;

    Position(Vector2 v);;

    Position(float x, float y);;
};

struct Triangle {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
};

struct Path {
    int index = 3;
    int indexMax = 3;
    bool valid = false;
    std::array<Vector2, 15> path{0};

    Vector2 getNext();

    [[nodiscard]] Vector2 getCurrent();

    [[nodiscard]] Vector2 getCurrent() const;

    [[nodiscard]] bool isValid() const;

    void invalidate();

    void validate();

    Path();
};

struct Radius {
    float value;

    operator float() const noexcept;
};

struct Grade {
    int value;

    operator int() const noexcept;
};


struct LookAngle {
    float value;

    operator float() const noexcept;

    LookAngle(float d);;
};


struct Spread {
    float value;

    operator float() const noexcept;
};

struct Speed {
    float value;
    float max;
    float actual{0};

    operator float() const noexcept;

    Speed(float x);

    Speed();;
};

struct Health {
    float value;
    float max;

    operator float() const noexcept;

    float operator-(const float x) const noexcept;

    void operator-=(const float x) noexcept;

    Health(float max);

    Health();
};

struct AttackTimer {
    Timer timer;
};

struct AttackSpeed {
    float value;

    operator float() const noexcept;
};

struct Damage {
    float value;

    operator float() const noexcept;
};

struct AttackRange {
    float value;

    operator float() const noexcept;
};

struct Pushback {
    float value;

    operator float() const noexcept;
};


struct ID {
    int value;

    operator int() const noexcept;
};

struct Experience {
    int value;

    operator int() const noexcept;

    void operator+=(const int x) noexcept;
};

struct Target {
    Vector2 value;

    operator Vector2() const noexcept;

    // Comparison operator
    bool operator<(const Target &other) const noexcept;
};

struct Projectile {
    Vector2 position;
    Vector2 target;
    float speed;
    float radius;
    float damage;
};

class AttackEffect {
    Timer m_timer;
    Vector2 m_center;
    Color m_color;
    float m_radius;
    float m_duration;
    float m_startAngle;
    float m_endAngle;

public:
    AttackEffect(float mDuration, Vector2 mCenter, const float &mRadius,
                 float mStartAngle, float mEndangle, const Color &mColor);

    void Draw() const;

    [[nodiscard]] bool Expired() const;
};

// p1, c1, c2, p2
struct LocalSpline {
    std::array<Vector2, 4> localPoints{};

    [[nodiscard]] std::array<Vector2, 4> getGlobal(Vector2 reference) const;
};

#endif //APTOMODO_COMPONENTS_H
