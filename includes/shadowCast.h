//
// Created by ginko on 11/21/24.
//

#ifndef SHADOW_CASTING_H
#define SHADOW_CASTING_H

#include "intGrid.h"

// TODO this can go in ai

void visualizeFOV(IntGrid &map, float startX, float startY, float radius,
                  float sectorStartAngle, float sectorEndAngle);

bool isTargetInFOV(IntGrid &map, float startX, float startY, float targetX, float targetY,
                   float radius, float sectorStartAngle, float sectorEndAngle);


#endif //SHADOW_CASTING_H
