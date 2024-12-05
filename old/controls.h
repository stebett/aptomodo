//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_CONTROLS_H
#define APTOMODO_CONTROLS_H


#include "components.h"

void playerAttack(entt::entity &player, Vector2 clickPosition);

void playerSecondaryAttack(entt::entity player);

void castFire(entt::entity player, Vector2 clickPosition);

void parseInput(entt::entity &player, Position &position, Camera2D &camera);

void updatePlayer(entt::entity &player, Position &position, Camera2D &camera);


#endif //APTOMODO_CONTROLS_H
