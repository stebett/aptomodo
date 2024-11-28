//
// Created by ginko on 11/26/24.
//

#include "space.h"

#include <components.h>
#include <constants.h>

constexpr int simPad{700};
constexpr Rectangle renderSpace{0, 0, Const::screenWidth, Const::screenHeight};
constexpr Rectangle simulationSpace{-simPad, -simPad, Const::screenWidth + simPad, Const::screenHeight + simPad};

bool pointInRec(const Vector2& point, const Rectangle& rec) {
    return (point.x >= rec.x) && (point.x < rec.width) && (point.y >= rec.y) && (point.y < rec.height);
}


bool inRenderSpace(const Vector2 &point) {
    return pointInRec(point, renderSpace);
}

bool inSimSpace(const Vector2 &point) {
    return pointInRec(point, simulationSpace);
}

namespace Space {
    void Update(entt::registry &registry, const Camera2D &camera) {
        registry.view<Position>().each([&camera, &registry](auto entity, auto position) {
            const Vector2 screenPos = GetWorldToScreen2D(position, camera);
            if (!inSimSpace(screenPos)) {
                registry.remove<ToRender>(entity);
                registry.remove<ToSimulate>(entity);
            } else {
                registry.emplace_or_replace<ToSimulate>(entity);
                if (inRenderSpace(screenPos)) {
                    registry.emplace_or_replace<ToRender>(entity);
                }
            }
        });
    }
}
