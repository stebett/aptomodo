//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_FACTORIES_H
#define ACEROLA_JAM0_FACTORIES_H

#include <LDtkLoader/Entity.hpp>
#include "components.h"

entt::entity spawnEnemy(entt::registry &registry, Position position, int level);
entt::entity spawnPlayer(entt::registry &registry, int level);
Camera2D spawnCamera();


#endif //ACEROLA_JAM0_FACTORIES_H
