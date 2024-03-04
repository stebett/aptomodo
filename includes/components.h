//
// Created by ginko on 01/03/24.
//

#ifndef ACEROLA_JAM0_COMPONENTS_H
#define ACEROLA_JAM0_COMPONENTS_H

#include <raylib.h>

// Tags
struct Player {
};
struct Enemy {
};
struct Living {
};

// Components
struct Position {
    float x;
    float y;

    operator Vector2() {
        return Vector2{x, y};
    }
};

struct Weapon {
    int damage;
    float range;
    float spread;
    float pushback;
};

struct Triangle {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
};

struct Health {
    float max;
    float value;
};

struct Radius {
    float value;
};



enum class State {
    casting,
    normal,
    pain,
};

struct PlayerState {
    State state;
};

struct Pain {
    float value;
};

enum class Level {
    One,
    Two,
    Three,
    Four,
    Five,
};


#endif //ACEROLA_JAM0_COMPONENTS_H
