//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_RENDERINGMANAGER_H
#define APTOMODO_RENDERINGMANAGER_H

#include <raylib.h>
#include <entt/entity/registry.hpp>

namespace Rendering {
    void UpdateCamera(Camera2D &camera, const Vector2 &playerPosition, float delta);

    void DrawLevel(const Camera2D &camera);

    void Draw(const Camera2D &camera, unsigned int frame); // TODO this should draw on a framebuffer
}


#endif // APTOMODO_RENDERINGMANAGER_H
