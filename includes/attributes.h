//
// Created by ginko on 04/05/24.
//

#ifndef APTOMODO_ATTRIBUTES_H
#define APTOMODO_ATTRIBUTES_H

#include <array>
#include <unordered_map>
#include <numeric>
#include <iostream>
#include "managers/parameters.h"
#include <vector>
#include "attributesConstants.h"

using namespace AttributeConstants;


class Attributes {
    int level{1};


    std::array<int, 6> values{};
    std::array<int, 18> subValues{};
    std::vector<Modifier> attributeModifiers{};

public:

    Attributes(int st, int in, int ag, int wi, int co, int pe);

    Attributes();

    void updateModifiers(const std::vector<Modifier> &modifiers);

    [[nodiscard]] int getLevel() const;

    [[nodiscard]] int get(AttributeName attr) const;

    [[nodiscard]] int get(SubAttributeName subattr) const;

    [[nodiscard]] float getMultiplied(SubAttributeName subattr);

    [[nodiscard]] int freeAttrPoints() const;

    [[nodiscard]] int freeSubAttrPoints(AttributeName attr) const;

    [[nodiscard]] int freeSubAttrPoints(SubAttributeName subattr) const;

    [[nodiscard]] bool outOfAttrPoints() const;

    [[nodiscard]] bool outOfSubAttrPoints(SubAttributeName subattr) const;

    [[nodiscard]] bool atMinAttrPoints(AttributeName attr) const;

    [[nodiscard]] bool atMinSubAttrPoints(SubAttributeName subattr) const;

    void decrease(AttributeName attr);

    void decrease(SubAttributeName subattr);

    void increase(AttributeName attr);

    void increase(SubAttributeName subattr);

    [[nodiscard]] int expForCurrentLevel() const;

    [[nodiscard]] int expToNextLevel() const;

    void levelUp();

};


#endif //APTOMODO_ATTRIBUTES_H
