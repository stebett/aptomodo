//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_FACTORIES_H
#define ACEROLA_JAM0_FACTORIES_H

#include "components.h"

entt::entity spawnEnemy(entt::registry &registry, Position position, Level level);
entt::entity spawnPlayer(entt::registry &registry);
Camera2D spawnCamera();

#endif //ACEROLA_JAM0_FACTORIES_H
