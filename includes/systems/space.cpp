//
// Created by ginko on 11/26/24.
//

#include "space.h"

#include <components.h>
#include <constants.h>

constexpr int simPad{100};
constexpr Rectangle renderSpace{0, 0, screenWidth, screenHeight};
constexpr Rectangle simulationSpace{-simPad, -simPad, screenWidth + simPad, screenHeight + simPad};

bool inRenderSpace(const Vector2 &point, const Camera2D &camera) {
    return CheckCollisionPointRec(GetWorldToScreen2D(point, camera), renderSpace);
}

bool inSimSpace(const Vector2 &point, const Camera2D &camera) {
    return CheckCollisionPointRec(GetWorldToScreen2D(point, camera), simulationSpace);
}

namespace Space {
    void Update(entt::registry &registry, const Camera2D &camera) {
        registry.view<Position>().each([&camera, &registry](auto entity, auto position) {
            if (!inSimSpace(position, camera)) {
                registry.remove<ToRender>(entity);
                registry.remove<ToSimulate>(entity);
            } else {
                registry.emplace_or_replace<ToSimulate>(entity);
                if (inRenderSpace(position, camera)) {
                    registry.emplace_or_replace<ToRender>(entity);
                }
            }
        });
    }
}
