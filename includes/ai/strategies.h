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
        Sequence chase = {};

        PlayerInView playerInView = {};
        PlayerInView playerInView2 = {};
        GetRandomTarget getRandomTarget = {};
        MoveTowardsTarget moveTowardsTarget = {};
        MoveTowardsTarget moveTowardsPlayer = {};
        PlayerInMeleeRange playerInMeleeRange = {};
        PlayerInMeleeRange playerInMeleeRange2 = {};
        GetPlayerTarget getPlayerTarget = {};
        AttackMelee attackMelee = {};

    public:
        Melee(entt::entity self);
    };

    struct Strategy {
        std::unique_ptr<BehaviorTree> behavior;
    };
}

#endif //APTOMODO_STRATEGIES_H
