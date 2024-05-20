//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_FACTORIES_H
#define APTOMODO_FACTORIES_H

#include <LDtkLoader/Entity.hpp>
#include "components.h"
#include <numeric>

entt::entity spawnRandomEnemy(entt::registry &registry);

void spawnEnemies(entt::registry &registry);

entt::entity spawnEnemy(entt::registry &registry, Vector2 position);

entt::entity spawnPlayer(entt::registry &registry);

Camera2D spawnCamera();

void spawnItems(entt::registry& registry);

#endif //APTOMODO_FACTORIES_H
