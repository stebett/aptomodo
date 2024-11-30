//
// Created by ginko on 11/28/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H


class Physics {
    static b2WorldId worldId;
    static std::unordered_map<int32_t, entt::entity> bodyMap;

public:
    enum ContactCategories {
        None = 0x00000000,
        Static = 0x00000001,
        Dynamic = 0x00000002,
        Enemy = 0x00000004,
        Player = 0x00000008,
    };

    static constexpr float timeStep{1.0 / 60.0};
    static constexpr int subStepCount{4};

    static void Instantiate();

    static b2BodyId CreateDynamicCircularBody(entt::entity entity, Vector2 position, float radius,
                                              ContactCategories category);

    // Todo these functions should return a bodyId, Physics should never touch the registry

    static void EmplaceStaticBody(Vector2 position, float side);

    static b2BodyId EmplaceSword(float half_width, float half_height);

    static void ConnectBodyToEntity(b2BodyId body, entt::entity entity);

    static void Update(entt::registry &registry);

    static void Step();

    static b2WorldId GetWorldID();
};


#endif //PHYSICS_H
