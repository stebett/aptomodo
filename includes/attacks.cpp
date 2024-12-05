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
            const auto transformer = Game::registry.get<LocalTransformSpline>(entity);
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
}
