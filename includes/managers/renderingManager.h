//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_RENDERINGMANAGER_H
#define APTOMODO_RENDERINGMANAGER_H

#include <components.h>
#include <raylib.h>
#include <entt/entity/registry.hpp>

class RenderingManager {
    explicit RenderingManager(entt::registry &registry);


    static RenderingManager *instance;
    static entt::registry *m_registry;

public:
    static RenderingManager &Instance();

    static void Instantiate(entt::registry &registry) {
        instance = new RenderingManager(registry);
    }

    static void UpdateCamera(Camera2D &camera, const Position &playerPosition);

    static void DrawLevel(const Camera2D &camera);

    static void Draw(const Camera2D &camera, unsigned int frame);

    ~RenderingManager() {
        delete instance;
    }
};


#endif // APTOMODO_RENDERINGMANAGER_H
