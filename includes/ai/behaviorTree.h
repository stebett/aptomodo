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

    [[nodiscard]] virtual const char *getName() const;

    [[nodiscard]] virtual Status getStatus();

    [[nodiscard]] virtual std::vector<Behavior *> getChildren() const;

    Status tick(entt::registry &registry, entt::entity self, entt::entity player);

protected:
    virtual void onInit() {
    };
    //
    virtual void onTerminate(Status) {
    };

    virtual Status update(entt::registry &registry, entt::entity self, entt::entity player) { return INVALID; }

public:
    Status status = INVALID;
    const char *name = "prova";
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
public:
    [[nodiscard]] std::vector<Behavior *> getChildren() const override;

    [[nodiscard]] const char *getName() const override { return name; }

protected:
    std::vector<Behavior *>::iterator currentChild;

    void onInit() override;

    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

public:
    const char *name = "Sequence";
};

class Fallback : public Composite {
public:
    [[nodiscard]] std::vector<Behavior *> getChildren() const override;

    [[nodiscard]] const char *getName() const override { return name; }

protected:
    std::vector<Behavior *>::iterator currentChild;

    void onInit() override;

    Status update(entt::registry &registry, entt::entity self, entt::entity player) override;

public:
    const char *name = "Fallback";
};


class BehaviorTree {
protected:
    Behavior *root;

public:
    void tick(entt::registry &registry, entt::entity self, entt::entity player);

    Behavior *getRoot();

    explicit BehaviorTree(Behavior *behavior, entt::registry &registry, entt::entity &self)
        : root(behavior), m_registry(registry), m_self(self) {
    };

private:
    entt::registry &m_registry;
    entt::entity &m_self;
};

void collectNodeStatus(Behavior *root, std::vector<std::pair<const char *, Status> > &result);


#endif //ENEMYSTATE_BEHAVIORTREE_H
