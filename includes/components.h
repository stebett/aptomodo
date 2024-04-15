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

struct ColorBB {
    Color value;
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


struct Radius {
    float value;
};


struct Spread {
    float value;
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
};

struct Health {
    float value;
    float max;
};

struct PhysicalResistance {
    float value;
};

struct MagicalResistance {
    float value;
};

struct Stamina {
    float value;
};

struct TimeLastAttack {
    float value;
};

struct AttackSpeed {
    float value;
};

struct Damage {
    float value;
};

struct AttackRange {
    float value;
};

struct Pushback {
    float value;
};



struct ID {
    int value;
};


namespace Animation {

    enum State {
        Stand,
        Walk,
        Attack,
    };
    using Map = std::unordered_map<Animation::State, std::vector<Rectangle>>;

}


#endif //ACEROLA_JAM0_COMPONENTS_H
