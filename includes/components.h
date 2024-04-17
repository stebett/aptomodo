//
// Created by ginko on 01/03/24.
//

#ifndef ACEROLA_JAM0_COMPONENTS_H
#define ACEROLA_JAM0_COMPONENTS_H

#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>
#include "timer.h"
#include <iostream>

// Tags
struct Player {
};
struct Enemy {
};
struct Living {
};
struct Chasing {
};

struct ColorBB {
    Color color;

    operator Color() const noexcept { return color; }
};

// Components
struct Position {
    float x;
    float y;

    operator Vector2() const noexcept { return Vector2{x, y}; }

    Position(Vector2 v) : x(v.x), y(v.y) {};

    Position(float x, float y) : x(x), y(y) {};
};

struct Triangle {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
};

struct Path {
    int index = 3;
    int indexMax = 3;
    std::array<Vector2, 7> path{0};

    Vector2 getNext() {
        index = std::min(index, indexMax);
        Vector2 result = path[index];
        index++;
        return result;
    }

    [[nodiscard]] Vector2 getCurrent() const {
        return path[std::min(index, indexMax)];
    }

    Vector2 getCurrent() {
        return path[std::min(index, indexMax)];
    }


    [[nodiscard]] bool isFinished() const {
        return index >= indexMax;
    }

    Path() : index(7), indexMax(7), path({0}) { std::cout << "INFO: path being created" << "\n"; };
};

struct Radius {
    float value;

    operator float() const noexcept { return value; }
};

struct LookAngle {
    float value;

    operator float() const noexcept { return value; }

    LookAngle(float d) : value(d) {};

};


struct Spread {
    float value;

    operator float() const noexcept { return value; }
};


/* Stats
 * John, [08/04/24 21:08]
Le statistiche saranno roba tipo
1)Velocità di movimento
2)Salute massima
3)Resistenza ai danni fisici
4)Resistenza ai danni magici
5)Stamina (o resistenza nel senso atletico)
6)Velocità di attacco
5)Danno fisico
6)Portata fisica
7)Mana massimo
8)Rigenerazione mana
9)danno magico
10)velocità di Cast
11)Portata magica
12)AoE Magico
(Anche AoE fisico, ma non ho voglia di riscrivere i numeri ahah)
*/

struct Speed {
    float value;

    operator float() const noexcept { return value; }
};

struct Health {
    float value;
    float max;


    operator float() const noexcept { return value; }

    float operator-(const float x) const noexcept { return value - x; }

    void operator-=(const float x) noexcept { value -= x; }

};

struct PhysicalResistance {
    float value;

    operator float() const noexcept { return value; }
};

struct MagicalResistance {
    float value;

    operator float() const noexcept { return value; }
};

struct Stamina {
    float value;

    operator float() const noexcept { return value; }
};

struct AttackTimer {
    Timer timer;
};

struct AttackSpeed {
    float value;

    operator float() const noexcept { return value; }
};

struct Damage {
    float value;

    operator float() const noexcept { return value; }
};

struct AttackRange {
    float value;

    operator float() const noexcept { return value; }
};

struct Pushback {
    float value;

    operator float() const noexcept { return value; }
};


struct ID {
    int value;

    operator int() const noexcept { return value; }
};


#endif //ACEROLA_JAM0_COMPONENTS_H
