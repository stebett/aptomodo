//
// Created by ginko on 11/29/24.
//

#ifndef ATTACKS_H
#define ATTACKS_H
#include <systems/physics.h>


namespace Attacks {
    class Sword {
    public:
        Sword(Vector2 anchor, float half_width, float half_height) {
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = (b2Vec2){anchor.x, anchor.y};
            b2BodyId bodyId = b2CreateBody(Physics::GetWorldID(), &bodyDef);
            b2Polygon box = b2MakeBox(half_width, half_height);
            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.density = 1.0f;
            shapeDef.friction = 0.3f;
            b2CreatePolygonShape(bodyId, &shapeDef, &box);

            //
            //     // Attach a fixture to the body
            //     b2Fix swordFixtureDef;
            //     swordFixtureDef.shape = &swordShape;
            //     swordFixtureDef.isSensor = true;  // Sensor ensures no physical collision response
            //     swordBody->CreateFixture(&swordFixtureDef);
            // }
        }
    };
}


#endif //ATTACKS_H
