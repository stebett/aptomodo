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

    Status tick();

protected:
    virtual void onInit() {};
//
    virtual void onTerminate(Status) {};

    virtual Status update() { return INVALID; }

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

    Status update() override;
};

class Fallback : public Composite {
protected:
    std::vector<Behavior *>::iterator currentChild;

    void onInit() override;

    Status update() override;
};



class BehaviorTree {
protected:
    Behavior *root;
public:
    void tick();

    explicit BehaviorTree(Behavior *behavior) : root(behavior) {};
};


#endif //ENEMYSTATE_BEHAVIORTREE_H
