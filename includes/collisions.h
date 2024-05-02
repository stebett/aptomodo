//
// Created by ginko on 02/03/24.
//

#ifndef ACEROLA_JAM0_COLLISIONS_H
#define ACEROLA_JAM0_COLLISIONS_H

#include "constants.h"

bool
CheckCollisionCircleTriangle(Vector2 center, float radius, Vector2 v1, Vector2 v2, Vector2 v3, float height);
void solveCircleRecCollision(Vector2 &futurePos, float radius);


#endif //ACEROLA_JAM0_COLLISIONS_H
