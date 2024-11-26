//
// Created by ginko on 11/26/24.
//

#include "space.h"

#include <components.h>
#include <constants.h>

bool isPointInRectangle(const Vector2 &point, const Rectangle &rec) {
    return (rec.x <= point.x) && (point.x <= rec.width) && (rec.y <= point.y) && (point.y <= rec.height);
}

namespace Space {
    void Update(entt::registry &registry, const Camera2D &camera) {
        registry.view<Position>().each([&camera, &registry](auto entity, auto position) {
            if (CheckCollisionPointRec(GetWorldToScreen2D(position, camera),
                                       {0, 0, screenWidth, screenHeight})) {
                registry.emplace_or_replace<ToRender>(entity);
            } else {
                registry.remove<ToRender>(entity);
            }
        });
    }
}
