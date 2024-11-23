//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_RENDERINGMANAGER_H
#define APTOMODO_RENDERINGMANAGER_H

#include <raylib.h>
#include <entt/entity/registry.hpp>

namespace RenderingManager {
    void UpdateCamera(Camera2D &camera, const Vector2 &playerPosition);

    void DrawLevel(const Camera2D &camera);

    void Draw(entt::registry &registry, const Camera2D &camera, unsigned int frame);
}


#endif // APTOMODO_RENDERINGMANAGER_H
