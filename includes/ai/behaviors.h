//
// Created by ginko on 05/06/24.
//

#ifndef APTOMODO_BEHAVIORS_H
#define APTOMODO_BEHAVIORS_H

#include "behaviorTree.h"

class PlayerInView : public Behavior {
    Status update() override;
};

class Patrol : public Behavior {
    Status update() override;
};

class PlayerInMeleeRange : public Behavior {
    Status update() override;
};

class Chase : public Behavior {
    Status update() override;
};

class AttackMelee : public Behavior {
    Status update() override;
};

#endif //APTOMODO_BEHAVIORS_H
