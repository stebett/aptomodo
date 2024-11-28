//
// Created by ginko on 11/28/24.
//

#include "physics.h"

b2WorldId Physics::worldId;


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
    b2Polygon dynamicBox = b2MakeBox(radius, radius);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 100.0f;
    b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
    registry.emplace<b2BodyId>(entity, bodyId);
}

void Physics::Update(entt::registry &registry) {
    b2World_Step(worldId, timeStep, subStepCount);
    registry.view<ToSimulate, b2BodyId, Position>().each([](auto entity, auto body, Position &position) {
        auto p = b2Body_GetPosition(body);
        position = {p.x, p.y};
    });
}
