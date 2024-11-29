//
// Created by ginko on 11/28/24.
//

#include "physics.h"
#include <components.h>

b2WorldId Physics::worldId;
std::unordered_map<int32_t, entt::entity> Physics::entityMap {};

void Physics::Instantiate() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    worldId = b2CreateWorld(&worldDef);
}

void Physics::EmplaceDynamicBody(entt::registry &registry, entt::entity entity, const Vector2 position, float radius) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){position.x, position.y};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    entityMap[bodyId.index1] = entity;
    const auto circle = b2Circle({0.0, 0.0}, radius);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;
    b2CreateCircleShape(bodyId, &shapeDef, &circle);
    registry.emplace<b2BodyId>(entity, bodyId);
}

void Physics::EmplaceStaticBody(const Vector2 position, float side) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){position.x + side / 2, position.y + side / 2};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    b2Polygon Box = b2MakeBox(side / 2, side / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(bodyId, &shapeDef, &Box);
}


void Physics::Update(entt::registry &registry) {
    const auto [moveEvents, moveCount] = b2World_GetBodyEvents(worldId);
    for (int i = 0; i < moveCount; ++i) {
        const b2BodyMoveEvent *event = moveEvents + i;
        b2BodyId bodyId = event->bodyId;
        b2Body_SetLinearVelocity(bodyId, {0.0f, 0.0f});
        Position &position = registry.get<Position>(entityMap[bodyId.index1]);
        const auto [x, y] = event->transform.p;
        position = {x, y};
    }
}

void Physics::Step() {
    b2World_Step(worldId, timeStep, subStepCount);
}


b2WorldId Physics::GetWorldID() {
    return worldId;
}
