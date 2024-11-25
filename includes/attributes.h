//
// Created by ginko on 04/05/24.
//

#ifndef APTOMODO_ATTRIBUTES_H
#define APTOMODO_ATTRIBUTES_H

#include "attributesConstants.h"

using namespace AttributeConstants;

/*
 * This class is responsible for holding all the attributes, sub-attributes and modifiers values for a given agent
 * It is quite heavy, so it would be better to not load it often to avoid filling the cache with crap
 * TODO Maybe we can add a flag that gets invalidated anytime attributes change, so that it recomputes damage, speed, etc. that then are loaded singularly where they are needed
 */
class Attributes {
    int level{1};

    std::array<int, 6> values{};
    std::array<int, 18> subValues{}; // TODO store chars instead of ints and add a check that it never goes above 256
    std::vector<Modifier> attributeModifiers{};

public:

    Attributes(int st, int in, int ag, int wi, int co, int pe);

    Attributes();

    void updateModifiers(const std::vector<Modifier> &modifiers);

    [[nodiscard]] int getLevel() const;

    [[nodiscard]] int get(AttributeName attr) const;

    [[nodiscard]] int get(SubAttributeName subattr) const;

    [[nodiscard]] float getMultiplied(SubAttributeName subattr) const;

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
