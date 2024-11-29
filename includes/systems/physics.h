//
// Created by ginko on 11/28/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H


class Physics {
    static b2WorldId worldId;
    static std::unordered_map<int32_t, entt::entity> bodyMap;

public:
    static constexpr float timeStep{1.0 / 60.0};
    static constexpr int subStepCount{4};

    static void Instantiate();

    static void EmplaceDynamicBody(entt::registry &registry, entt::entity entity, Vector2 position, float radius);

    // Todo these functions should return a bodyId, Physics should never touch the registry

    static void EmplaceStaticBody(Vector2 position, float side);

    static void EmplaceSword(entt::registry &registry, entt::entity entity, Vector2 anchor, float half_width,
                             float half_height);

    static void Update(entt::registry &registry);

    static void Step();

    static b2WorldId GetWorldID();
};


#endif //PHYSICS_H
