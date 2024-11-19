//
// Created by ginko on 30/05/24.
//

#ifndef ENEMYSTATE_BEHAVIORTREE_H
#define ENEMYSTATE_BEHAVIORTREE_H

#include <vector>

enum Status {
    INVALID,
    RUNNING,
    SUCCESS,
    FAILURE,
};

class Behavior {
public:
    Behavior() = default;

    virtual ~Behavior() = default;

    Status tick(entt::registry &registry, entt::entity self, entt::entity player);

protected:
    virtual void onInit() {
    };
    //
    virtual void onTerminate(Status) {
    };

    virtual Status update(entt::registry &registry, entt::entity self, entt::entity player) { return INVALID; }

private:
    Status status = INVALID;
};


class Composite : public Behavior {
public:
    void addChild(Behavior *);

    //    void removeChild(Behavior *);
    //
    //    void clearChildren();

protected:
    std::vector<Behavior *> children;
};

class Sequence : public Composite {
protected:
    std::vector<Behavior *>::iterator currentChild;

    void onInit() override;

    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;
};

class Fallback : public Composite {
protected:
    std::vector<Behavior *>::iterator currentChild;

    void onInit() override;

    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;
};


class BehaviorTree {
protected:
    Behavior *root;

public:
    void tick(entt::registry &registry, entt::entity self, entt::entity player);

    explicit BehaviorTree(Behavior *behavior, entt::registry &registry, entt::entity &self)
        : root(behavior), m_registry(registry), m_self(self) {
    };

private:
    entt::registry &m_registry;
    entt::entity &m_self;
};


#endif //ENEMYSTATE_BEHAVIORTREE_H
