//
// Created by ginko on 05/06/24.
//

#include "pch.h"
#include "behaviors.h"



Status PlayerInView::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInView" << "\n";
    return SUCCESS;
}

Status Patrol::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Patrol" << "\n";
    return SUCCESS;
}

Status PlayerInMeleeRange::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "PlayerInMeleeRange" << "\n";
    return SUCCESS;
}

Status Chase::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "Chase" << "\n";
    return SUCCESS;
}

Status AttackMelee::update(entt::registry &registry, entt::entity self, entt::entity player) {
    std::cout << "AttackMelee" << "\n";
    return SUCCESS;
}
