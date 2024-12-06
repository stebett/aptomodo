//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_FACTORIES_H
#define APTOMODO_FACTORIES_H

#include <managers/levelManager.h>


Camera2D spawnCamera();

void spawnEntities(const std::vector<Level::Entity> &);
entt::entity spawnEnemyByName(Vector2 position, const std::string &enemyName);

entt::entity spawnPlayer();


#endif //APTOMODO_FACTORIES_H
