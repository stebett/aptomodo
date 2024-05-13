//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_FACTORIES_H
#define ACEROLA_JAM0_FACTORIES_H

#include <LDtkLoader/Entity.hpp>
#include "components.h"
#include <numeric>

entt::entity spawnRandomEnemy(entt::registry &registry);

void spawnEnemies(entt::registry &registry);

entt::entity spawnEnemy(entt::registry &registry, Vector2 position);

entt::entity spawnPlayer(entt::registry &registry);

Camera2D spawnCamera();

void spawnAmulet(entt::registry& registry);

#endif //ACEROLA_JAM0_FACTORIES_H
