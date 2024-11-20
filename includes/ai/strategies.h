//
// Created by ginko on 05/06/24.
//

#ifndef APTOMODO_STRATEGIES_H
#define APTOMODO_STRATEGIES_H

#include "behaviorTree.h"
#include "behaviors.h"

namespace Strategy {

    class Melee : public BehaviorTree {
    private:
        Sequence root = {};
        Fallback fallback1 = {};
        Fallback fallback2 = {};
        Sequence patrol = {};

        PlayerInView playerInView = {};
        GetRandomTarget getRandomTarget = {};
        MoveTowardsTarget moveTowardsTarget = {};
        PlayerInMeleeRange playerInMeleeRange = {};
        Chase chase = {};
        AttackMelee attackMelee = {};

    public:
        Melee(entt::registry& registry, entt::entity self) : BehaviorTree(&root, registry, self) {
            fallback1.addChild(&playerInView);
            fallback1.addChild(&patrol);
            patrol.addChild(&getRandomTarget);
            patrol.addChild(&moveTowardsTarget);

            fallback2.addChild(&playerInMeleeRange);
            fallback2.addChild(&chase);

            root.addChild(&fallback1);
            root.addChild(&fallback2);
            root.addChild(&attackMelee);

        }

    };
    struct Strategy {
        std::unique_ptr<BehaviorTree> behavior;
    };
}

#endif //APTOMODO_STRATEGIES_H
