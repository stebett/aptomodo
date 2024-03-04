//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_LEVELS_H
#define ACEROLA_JAM0_LEVELS_H

#include <entt/entity/registry.hpp>
#include "components.h"
#include "rendering.h"


void playLevel(entt::registry &registry, GameScene &scene, int level);

float radiusByLevel(int level);

Weapon swordByLevel(int level);

#endif //ACEROLA_JAM0_LEVELS_H
