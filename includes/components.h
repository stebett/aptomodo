//
// Created by ginko on 01/03/24.
//

#ifndef ACEROLA_JAM0_COMPONENTS_H
#define ACEROLA_JAM0_COMPONENTS_H

// Tags
struct Player {};
struct Enemy {};
// Components
struct Position {
    float x;
    float y;
};

struct Weapon {
    int damage;
    int range;
};

struct Triangle {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
};



#endif //ACEROLA_JAM0_COMPONENTS_H
