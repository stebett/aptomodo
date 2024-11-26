//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_FACTORIES_H
#define APTOMODO_FACTORIES_H
#include <managers/levelManager.h>



Camera2D spawnCamera();

void spawnEntities(entt::registry &registry, const std::vector<Level::Entity>&);

entt::entity spawnPlayer(entt::registry &registry);


#endif //APTOMODO_FACTORIES_H
