//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_CONTROLS_H
#define ACEROLA_JAM0_CONTROLS_H


#include "components.h"
#include "constants.h"

void playerAttack(entt::registry &registry, entt::entity &player, Vector2 clickPosition);

void playerSecondaryAttack(entt::registry &registry, entt::entity player);

void castFire(entt::registry &registry, entt::entity player, Vector2 clickPosition);

void parseInput(entt::registry &registry, entt::entity &player, Position &position, Camera2D &camera);



#endif //ACEROLA_JAM0_CONTROLS_H
