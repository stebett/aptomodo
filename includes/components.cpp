//
// Created by ginko on 11/23/24.
//

#include <components.h>

Chasing::Chasing(const float p_duration): timer(Timer()), duration(p_duration) {
}

bool Chasing::isChasing() const { return timer.ElapsedSeconds() < duration; }

ColorBB::operator Color() const noexcept { return color; }

Position::operator Vector2() const noexcept { return Vector2{x, y}; }

Position::Position(Vector2 v): x(v.x), y(v.y) {
}

Position::Position(float x, float y): x(x), y(y) {
}

Speed::operator float() const noexcept { return value; }

Speed::Speed(float x): value(x), max(x) {
}

Health::operator float() const noexcept { return value; }

float Health::operator-(const float x) const noexcept { return value - x; }

void Health::operator-=(const float x) noexcept { value -= x; }

Health::Health(float value, const float max): value(value), max(max) {
}


AttackSpeed::operator float() const noexcept { return value; }

Damage::operator float() const noexcept { return value; }

AttackRange::operator float() const noexcept { return value; }

Pushback::operator float() const noexcept { return value; }

ID::operator int() const noexcept { return value; }

Experience::operator int() const noexcept { return value; }

void Experience::operator+=(const int x) noexcept { value += x; }

Target::operator Vector2() const noexcept { return value; }

bool Target::operator<(const Target &other) const noexcept {
    if (value.x != other.value.x) {
        return value.x < other.value.x;
    }
    return value.y < other.value.y;
}

Vector2 Path::getNext() {
    index = std::min(index, indexMax);
    Vector2 result = path[index];
    index++;
    return result;
}

Vector2 Path::getCurrent() const {
    return path[std::min(index, indexMax)];
}

Vector2 Path::getCurrent() {
    return path[std::min(index, indexMax)];
}

bool Path::isValid() const {
    return valid && (index < indexMax);
}

void Path::invalidate() {
    valid = false;
}

void Path::validate() {
    valid = true;
}

Path::Path(): index(7), indexMax(7), path({0}) {
}

Radius::operator float() const noexcept { return value; }

LookAngle::operator float() const noexcept { return value; }

LookAngle::LookAngle(float d): value(d) {
}

Spread::operator float() const noexcept { return value; }

AttackEffect::AttackEffect(float mDuration, Vector2 mCenter, const float &mRadius,
                           float mStartAngle, float mEndangle, const Color &mColor) : m_duration(mDuration),
                                                                                      m_center(mCenter),
                                                                                      m_radius(mRadius),
                                                                                      m_color(mColor),
                                                                                      m_startAngle(mStartAngle),
                                                                                      m_endAngle(mEndangle) {
    m_timer.Reset();
}

void AttackEffect::Draw() const {
    DrawCircleSector({m_center.x, m_center.y}, m_radius, m_startAngle, m_endAngle, 2,
                     ColorAlpha(m_color, 1 - m_timer.ElapsedMillis() / m_duration));
}

bool AttackEffect::Expired() const { return m_timer.ElapsedMillis() > m_duration; }
