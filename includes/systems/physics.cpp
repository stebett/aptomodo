//
// Created by ginko on 11/28/24.
//

#include "physics.h"
#include <components.h>

#include "attacks.h"

b2WorldId Physics::worldId;
std::unordered_map<int32_t, entt::entity> Physics::bodyMap{};


void Physics::Instantiate() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    worldId = b2CreateWorld(&worldDef);
}

b2BodyId Physics::CreateDynamicCircularBody(const entt::entity entity, const Vector2 position,
                                            const float radius, const ContactCategories category) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){position.x, position.y};

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = category;
    shapeDef.friction = 0.0f;

    const auto bodyId = b2CreateBody(worldId, &bodyDef);
    const auto circle = b2Circle({0.0, 0.0}, radius);
    b2CreateCircleShape(bodyId, &shapeDef, &circle);
    bodyMap[bodyId.index1] = entity;

    return bodyId;
}

void Physics::EmplaceSword(entt::registry &registry, entt::entity entity, b2BodyId spawningBody, Vector2 anchor,
                           float half_width,
                           float half_height, float degrees) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    float radians = (degrees-90) * M_PI / 180.0;
    float radiansEnd = (degrees) * M_PI / 180.0;
    bodyDef.rotation = {cos(radians), sin(radians)};
    bodyDef.position = (b2Vec2){anchor.x + (10+half_height)*bodyDef.rotation.c, anchor.y + (10+half_height)*bodyDef.rotation.s};


    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);


    bodyMap[bodyId.index1] = entity;
    // b2Polygon box = b2MakeOffsetBox(half_width, half_height, {0, 0},
                                    // b2Rot(radians, radians));
    b2Polygon box = b2MakeBox(half_width, half_height);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.1f;
    shapeDef.isSensor = true;
    shapeDef.filter.maskBits = Physics::Enemy;
    b2CreatePolygonShape(bodyId, &shapeDef, &box);
    registry.emplace<b2BodyId>(entity, bodyId);


    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
    jointDef.bodyIdA = bodyId;
    jointDef.bodyIdB = spawningBody;
    // jointDef.localAnchorA = {0.0f, 0.0f};
    jointDef.localAnchorA = {0, -half_height - 10.0f};
    jointDef.localAnchorB = {0.0f, 0.0f};
    b2CreateRevoluteJoint(worldId, &jointDef);

    // jointDef.enableMotor = true;
    // jointDef.motorSpeed = 10.0f;
    // jointDef.maxMotorTorque =10.0f;
    //
    // jointDef.enableLimit = true;
    // jointDef.lowerAngle = radiansEnd;// * DEGTORAD;
    // jointDef.upperAngle = radians;

    // b2Body_ApplyAngularImpulse(bodyId, 1.2, true);
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
        const auto entity = bodyMap[bodyId.index1];
        if (registry.all_of<Position>(entity)) {
            Position &position = registry.get<Position>(entity);
            const auto [x, y] = event->transform.p;
            position = {x, y};
            b2Body_SetLinearVelocity(bodyId, {0.0f, 0.0f});
        }
    }
    registry.clear<Attacks::Hit>();
    const b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        const b2SensorBeginTouchEvent *beginTouch = sensorEvents.beginEvents + i;
        const auto weapon = bodyMap[b2Shape_GetBody(beginTouch->sensorShapeId).index1];
        const auto target = bodyMap[b2Shape_GetBody(beginTouch->visitorShapeId).index1];
        const auto sword = registry.get<Attacks::Attack>(weapon);
        registry.emplace_or_replace<Attacks::Hit>(target);
        auto &health = registry.get<Health>(target);
        health -= sword.damage;
    }
}

void Physics::Step() {
    b2World_Step(worldId, timeStep, subStepCount);
}


b2WorldId Physics::GetWorldID() {
    return worldId;
}
