//
// Created by ginko on 05/06/24.
//

#include <iostream>
#include "behaviors.h"



Status PlayerInView::update() {
    std::cout << "PlayerInView" << "\n";
    return SUCCESS;
}

Status Patrol::update() {
    std::cout << "Patrol" << "\n";
    return SUCCESS;
}

Status PlayerInMeleeRange::update() {
    std::cout << "PlayerInMeleeRange" << "\n";
    return SUCCESS;
}

Status Chase::update() {
    std::cout << "Chase" << "\n";
    return SUCCESS;
}

Status AttackMelee::update() {
    std::cout << "AttackMelee" << "\n";
    return SUCCESS;
}
