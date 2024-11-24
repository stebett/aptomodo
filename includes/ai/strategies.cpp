//
// Created by ginko on 11/24/24.
//

#include "strategies.h"

Strategy::Melee::Melee(entt::registry &registry, entt::entity self): BehaviorTree(&root, registry, self) {
    fallback1.addChild(&playerInView);
    fallback1.addChild(&patrol);
    patrol.addChild(&getRandomTarget);
    patrol.addChild(&moveTowardsTarget);
    patrol.addChild(&playerInView2);

    fallback2.addChild(&playerInMeleeRange);
    fallback2.addChild(&chase);
    chase.addChild(&getPlayerTarget);
    chase.addChild(&moveTowardsPlayer);
    chase.addChild(&playerInMeleeRange2);


    root.addChild(&fallback1);
    root.addChild(&fallback2);
    root.addChild(&attackMelee);

}
