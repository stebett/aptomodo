//
// Created by ginko on 21/05/24.
//
//
// Created by ginko on 21/05/24.
//

#include "attributes.h"
#include <managers/parameters.h>

Attributes::Attributes(int st, int in, int ag, int wi, int co, int pe) {
    values[strength] = st;
    values[intelligence] = in;
    values[agility] = ag;
    values[willpower] = wi;
    values[coordination] = co;
    values[perception] = pe;


    for (auto attr: attributeVec)
        for (auto subattr: subAttrByAttr[attr])
            subValues[subattr] = Params::attributes.pointsAtStart;
}

Attributes::Attributes() : Attributes(1, 1, 1, 1, 1, 1) {}

void Attributes::updateModifiers(const std::vector<Modifier> &modifiers) {
    attributeModifiers = modifiers;
}

[[nodiscard]] int Attributes::getLevel() const { return level; }

[[nodiscard]] int Attributes::get(const AttributeName attr) const {
    int value = values[attr];
    for (auto modifier: attributeModifiers) {
        if (modifier.name == attr) {
            switch (modifier.operation) {
                case ModifierOperator::MUL:
                    value *= modifier.value;
                    break;
            }
        }
        if (modifier.name == attr) {
            switch (modifier.operation) {
                case ModifierOperator::MUL:
                    value *= modifier.value;
                    break;
            }
        }
    }
    return value;
}

[[nodiscard]] int Attributes::get(const SubAttributeName subattr) const { return subValues[subattr]; }

[[nodiscard]] float Attributes::getMultiplied(const SubAttributeName subattr) const {
    //TODO add a check that the attr is not too small to support the subattrs
//        return subValuesMultiplied[subattr]; ?
    return Params::attributes.subAttrAtStart[subattr] + static_cast<float>(subValues[subattr]) * Params::attributes.subAttrMultipliers[subattr];
}

int Attributes::freeAttrPoints() const {
    return (level - 1) * Params::attributes.pointsByLevel - std::accumulate(values.begin(), values.end(), 0) + 6;
}

int Attributes::freeSubAttrPoints(AttributeName attr) const {
    int attrValue = get(attr);
    int subAttrTotal = 0;
    for (auto sa: subAttrByAttr.at(attr)) subAttrTotal += subValues[sa] - Params::attributes.pointsAtStart;
    return Params::attributes.pointsByAttr * attrValue - subAttrTotal;
}

int Attributes::freeSubAttrPoints(SubAttributeName subattr) const {
    AttributeName attr = attrBySubAttr.at(subattr);
    return freeSubAttrPoints(attr);
}

bool Attributes::outOfAttrPoints() const {
    return freeAttrPoints() <= 0;
}

bool Attributes::outOfSubAttrPoints(const SubAttributeName subattr) const {
    return freeSubAttrPoints(subattr) <= 0;
}

bool Attributes::atMinAttrPoints(AttributeName attr) const {
    int subAttrTotal = 0;
    for (auto sa: subAttrByAttr.at(attr)) subAttrTotal += subValues[sa] - Params::attributes.pointsAtStart;
    return values[attr] <= 1 ||
           subAttrTotal > Params::attributes.pointsByAttr * values[attr] - Params::attributes.pointsByAttr;
}


bool Attributes::atMinSubAttrPoints(SubAttributeName subattr) const {
    return subValues[subattr] <= 0;
}


void Attributes::decrease(const AttributeName attr) {
    if (atMinAttrPoints(attr)) return;
    values[attr] -= 1;
}

void Attributes::decrease(const SubAttributeName subattr) {
    if (atMinSubAttrPoints(subattr)) return;
    subValues[subattr] -= 1;
}

void Attributes::increase(const AttributeName attr) {
    if (outOfAttrPoints()) {
        std::cout << "You don't have free attribute points\n";
        return;
    }
    values[attr] += 1;
}

void Attributes::increase(const SubAttributeName subattr) {
    if (outOfSubAttrPoints(subattr)) {
        std::cout << "You don't have free subattribute points\n";
        return;
    }
    subValues[subattr] += 1;
}


int Attributes::expForCurrentLevel() const {
    int total = 0;
    int l = level - 1;
    while (l > 0) {
        total += l * Params::attributes.expByLevel;
        l -= 1;
    }
    return total;
}

int Attributes::expToNextLevel() const {
    return expForCurrentLevel() + level * Params::attributes.expByLevel;
}

void Attributes::levelUp() {
    level += 1;
}

