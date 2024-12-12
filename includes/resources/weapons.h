//
// Created by ginko on 12/11/24.
//

#ifndef APTOMODO_WEAPONS_H
#define APTOMODO_WEAPONS_H

#include "gui/animationEditor.h"
#include <concepts>

struct Weapon {
    std::string name;
    int damage;
    Transform transform;
    std::shared_ptr<Animation> animation;

    template<typename S>
    requires std::convertible_to<S, std::string>
    Weapon(S&& name): name(std::forward<S>(name)) {

    };
};


#endif //APTOMODO_WEAPONS_H
