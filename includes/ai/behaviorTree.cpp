//
// Created by ginko on 30/05/24.
//

#include "behaviorTree.h"

const char * Behavior::getName() const {
    return name;
}

Status Behavior::getStatus()  {
    const Status s = status;
    status = INVALID; // TODO this will cause bugs 100%
    return s;
}

std::vector<Behavior *> Behavior::getChildren() const {
    return {};
}

Status Behavior::tick(entt::registry& registry, entt::entity self, entt::entity player) {
    if (status != RUNNING) onInit();
    status = update(registry, self, player);
    if (status != RUNNING) onTerminate(status);
    return status;
}


void Composite::addChild(Behavior * behavior) {
    children.emplace_back(behavior);
}

std::vector<Behavior *> Sequence::getChildren() const {
    return children;
};

std::vector<Behavior *> Fallback::getChildren() const {
    return children;
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

        if (++currentChild == children.end()) {
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
        if (++currentChild == children.end()) {
            return FAILURE;
        }
    }
    return INVALID;
}

void BehaviorTree::tick(entt::registry& registry, entt::entity self, entt::entity player) {
    root -> tick(registry, self, player);
}

Behavior * BehaviorTree::getRoot() {
    return root;
}

void collectNodeStatus(Behavior* root, std::vector<std::pair<const char*, Status>> &result) {
    result.emplace_back(root->getName(), root->getStatus());

    // If the node has children, traverse them
    for (const auto& child : root->getChildren()) {
        collectNodeStatus(child, result);
    }
}

