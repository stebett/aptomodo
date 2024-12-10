//
// Created by ginko on 11/29/24.
//

#include "attacks.h"
#include "managers/game.h"

#include <timer.h>
#include <systems/physics.h>

auto createShapeHelper = [](b2BodyId bodyId, b2Polygon box) {
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;
    shapeDef.filter.maskBits = Physics::Enemy;
    return b2CreatePolygonShape(bodyId, &shapeDef, &box);
};

namespace Attacks {
    void Update() {
        const auto now = GetTime();
        Game::registry.view<Attacks::Attack>().each([now](auto entity, const auto attack) {
            auto timer{Game::registry.get<PassiveTimer>(entity)};
            const auto t = (now - timer.start) / timer.duration;
            auto bodyCouple{Game::registry.get<BodyCouple>(entity)};
            const auto transformer = Game::registry.get<Transform>(entity);
            auto oldTransform{transformer.get(t)};
            auto ownerTransform{b2Body_GetTransform(bodyCouple.owner)};
            auto newTransform = b2MulTransforms(ownerTransform, oldTransform);
            b2Body_SetTransform(bodyCouple.weapon, newTransform.p, newTransform.q);

            b2ShapeId shape;
            b2Body_GetShapes(bodyCouple.weapon, &shape, 1);
            b2DestroyShape(shape, false);
            createShapeHelper(bodyCouple.weapon, b2MakeBox(transformer.getDim1(t), transformer.getDim2(t)));

            if (now - timer.start > timer.duration) {
                Physics::DestroyBody(bodyCouple.weapon);;
                Game::registry.destroy(entity);
            }
        });
    }

    Attack::Attack(const float damage) : damage(damage) {
    }

    b2Rot Transform::startAngle() const { return b2Rot(cos(startRadians), sin(startRadians)); }

    b2Rot Transform::endAngle() const { return b2Rot(cos(endRadians), sin(endRadians)); }

    Transform::Transform(LocalSpline localSpline) : spline(localSpline) {
    }

    b2Transform Transform::get(const float t) const {
        const auto bezier = spline.getLocalBezier();
        const auto eased_t = easingSpeed.valueAt(t);
        const Math::Vec2 p = bezier.valueAt(eased_t);
        const Math::Vec2 norm = bezier.normalAt(eased_t);
        if (isnan(norm.x) || isnan(norm.y)) {
            return {p, startAngle()};
        }
        const auto radians = atan2(norm.y, norm.x);
        const auto eased_angle_t = easingAngle.valueAt(t);

        const auto q = b2MulRot(b2Rot(cos(radians), sin(radians)),
                                b2NLerp(startAngle(), endAngle(), eased_angle_t));
        return {p, q};
    }

    float Transform::getDim1(const float t) const {
        return Lerp(startDim1, endDim1, easingDim1.valueAt(t));
    }

    float Transform::getDim2(const float t) const {
        return Lerp(startDim2, endDim2, easingDim2.valueAt(t));
    }
}
