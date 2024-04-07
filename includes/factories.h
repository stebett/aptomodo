//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_FACTORIES_H
#define ACEROLA_JAM0_FACTORIES_H

#include <LDtkLoader/Entity.hpp>
#include "components.h"
#include "rendering.h"

entt::entity spawnRandomEnemy(entt::registry &registry, Level::Variables &level);
entt::entity spawnEnemy(entt::registry &registry, Position position, Level::Variables &level);
entt::entity spawnPlayer(entt::registry &registry, GameScene *scene, Level::Variables &level);
Camera2D spawnCamera();

#endif //ACEROLA_JAM0_FACTORIES_H
