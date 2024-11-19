//
// Created by ginko on 30/05/24.
//

#include "behaviorTree.h"

Status Behavior::tick(entt::registry& registry, entt::entity self, entt::entity player) {
    if (status != RUNNING) onInit();
    status = update(registry, self, player);
    if (status != RUNNING) onTerminate(status);
    return status;
}


void Composite::addChild(Behavior * behavior) {
    children.emplace_back(behavior);
};

//void Composite::removeChild(Behavior *) {};
//
//void Composite::clearChildren() {};


void Sequence::onInit() {
    currentChild = children.begin();
}

Status Sequence::update(entt::registry& registry, entt::entity self, entt::entity player) {
    while (true) {
        Status s = (*currentChild)->tick(registry, self, player);
        if (s != SUCCESS) {
            return s;
        }

        currentChild++;
        if (currentChild == children.end()) {
            return SUCCESS;
        }
    }
    return INVALID;
}

void Fallback::onInit() {
    currentChild = children.begin();
}

Status Fallback::update(entt::registry& registry, entt::entity self, entt::entity player) {
    while (true) {
        Status s = (*currentChild)->tick(registry, self, player);
        if (s != FAILURE) {
            return s;
        }
        if (currentChild == children.end()) {
            return FAILURE;
        }
        currentChild++;
    }
    return INVALID;
}

void BehaviorTree::tick(entt::registry& registry, entt::entity self, entt::entity player) {
    root -> tick(registry, self, player);
}
