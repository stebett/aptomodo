//
// Created by ginko on 17/04/24.
//

#ifndef ACEROLA_JAM0_RENDERINGMANAGER_H
#define ACEROLA_JAM0_RENDERINGMANAGER_H

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

    static void Draw(const Camera2D &camera, unsigned int frame);

    ~RenderingManager() {
        delete instance;
    }
};




#endif // ACEROLA_JAM0_RENDERINGMANAGER_H
