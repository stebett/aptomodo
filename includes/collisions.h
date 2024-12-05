//
// Created by ginko on 02/03/24.
//

#ifndef APTOMODO_COLLISIONS_H
#define APTOMODO_COLLISIONS_H

#include "constants.h"

bool
CheckCollisionCircleTriangle(Vector2 center, float radius, Vector2 v1, Vector2 v2, Vector2 v3, float height);

void solveCircleRecCollision(Vector2 &futurePos, float radius);


#endif //APTOMODO_COLLISIONS_H
