//
// Created by ginko on 11/21/24.
//

#ifndef SHADOW_CASTING_H
#define SHADOW_CASTING_H
#include "intGrid.h"

void do_fov(IntGrid &map, float startX, float startY, float radius,
            float sectorStartAngle, float sectorEndAngle);


#endif //SHADOW_CASTING_H
