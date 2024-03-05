//
// Created by ginko on 02/03/24.
//

#include "levels.h"
#include "controls.h"
#include "constants.h"
#include <raylib.h>



Weapon swordByLevel(int level) {
    switch (level) {
        case 1:
            return {10, 100, 25, 150};
        case 2:
            return {10, 100, 25, 150};
        case 3:
            return {10, 100, 25, 150};
        case 4:
            return {10, 100, 25, 150};
        case 5:
            return {10, 100, 25, 150};
        default:
            return {10, 100, 90, 2000};
    }
}

float radiusByLevel(int level) {
    switch (level) {
        case 1:
            return 15;
        case 2:
            return 25;
        case 3:
            return 35;
        case 4:
            return 45;
        case 5:
            return 55;
        default:
            return 1;
    }
}

float painByLevel(int level) {
    switch (level) {
        case 1:
            return 200;
        case 2:
            return 300;
        case 3:
            return 400;
        case 4:
            return 500;
        case 5:
            return 600;
        default:
            return 999;
    }
}

Color colorByLevel(int level) {
    switch (level) {
        case 1:
            return BLACK;
        case 2:
            return BLUE;
        case 3:
            return GREEN;
        case 4:
            return GRAY;
        case 5:
            return RED;
        default:
            return WHITE;
    }
}

void playLevel(entt::registry &registry, GameScene &scene, int level) {

}
