//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_FACTORIES_H
#define ACEROLA_JAM0_FACTORIES_H

#include <LDtkLoader/Entity.hpp>
#include "components.h"

entt::entity spawnRandomEnemy(entt::registry &registry);
void spawnEnemies(entt::registry &registry);

entt::entity spawnEnemy(entt::registry &registry, Vector2 position);
entt::entity spawnPlayer(entt::registry &registry);
Camera2D spawnCamera();

namespace Attr {
    class Strength {
        int value;
        int damagePhysical;
        int health;
        int resistancePhysical;
    };
}

#endif //ACEROLA_JAM0_FACTORIES_H
