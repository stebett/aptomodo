//
// Created by ginko on 11/28/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include <components.h>
#include <box2d/box2d.h>

class Physics {
    static b2WorldId worldId;

public:
    static constexpr float timeStep {1.0/60.0};
    static constexpr int subStepCount {4};
    static void Instantiate();

    static void EmplaceDynamicBody(entt::registry &registry, entt::entity entity, const Vector2 position, float radius);

    static void Update(entt::registry &registry);
};


#endif //PHYSICS_H
