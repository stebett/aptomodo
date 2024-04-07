//
// Created by ginko on 01/03/24.
//

#ifndef ACEROLA_JAM0_COMPONENTS_H
#define ACEROLA_JAM0_COMPONENTS_H

#include <raylib.h>
#include <unordered_map>
#include <vector>


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

    Position(Vector2 v) : x(v.x), y(v.y) {}

    Position(float x, float y) : x(x), y(y) {}
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
    float value;
    float max;
};

struct Radius {
    float value;
};

struct Speed {
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
    float max;
};

namespace Level {
    struct Variables {
        int id;
        Pain pain;
        Weapon sword;
        Radius radius;
        Color color;
    };
}
namespace Animation {

    enum State {
        Stand,
        Walk,
        Attack,
    };
    using Map = std::unordered_map<Animation::State, std::vector<Rectangle>>;

}


#endif //ACEROLA_JAM0_COMPONENTS_H
