//
// Created by ginko on 05/06/24.
//

#ifndef APTOMODO_BEHAVIORS_H
#define APTOMODO_BEHAVIORS_H

#include "behaviorTree.h"

class PlayerInView : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "PlayerInView";
};



class PlayerInMeleeRange : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "PlayerInMeleeRange";
};

class AttackMelee : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "AttackMelee";
};

class GetRandomTarget : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "GetRandomPosition";
};

class MoveTowardsTarget : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "MoveTowardsTarget";
};

class GetPlayerTarget : public Behavior {
    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

    [[nodiscard]] const char *getName() const override { return name; }

public:
    const char *name = "GetPlayerTarget";
};


#endif //APTOMODO_BEHAVIORS_H
