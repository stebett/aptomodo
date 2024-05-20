//
// Created by ginko on 04/05/24.
//

#ifndef APTOMODO_ATTRIBUTES_H
#define APTOMODO_ATTRIBUTES_H

#include <array>
#include <unordered_map>
#include <numeric>
#include <iostream>
#include "config.h"
#include <vector>
#include "items.h"

namespace AttributeConstants {

    static constexpr int expByLevel{100};
    static constexpr int pointsByLevel{3};
    static constexpr int pointsByAttr{3};
    static constexpr int pointsAtStart{10};

    enum class ModifierOperator {
        ADD,
        MUL
    };

    constexpr std::array<char const *, 2> operatorString{
            "ADD",
            "MUL"
    };

    enum SubAttributeName {
        damagePhysical,
        health,
        resistancePhysical,

        damageMagical,
        mana,
        resistanceMagical,

        speed,
        attackSpeed,
        spellSpeed,

        regenerationMana,
        regenerationStamina,
        resistanceStatus,

        spread,
        range,
        dodgeRange,

        visualRange,
        critChance,
        critMultiplier,
    };
    enum AttributeName {
        strength,
        intelligence,
        agility,
        willpower,
        coordination,
        perception,
    };
    struct Modifier {
        AttributeName name;
        float value;
        ModifierOperator operation;
    };
    constexpr static std::array<AttributeName, 6> attributeVec{strength, intelligence, agility, willpower, coordination,
                                                               perception,};
    constexpr static std::array<AttributeConstants::SubAttributeName, 18> subAttributeVec{
            damagePhysical, health, resistancePhysical,
            damageMagical, mana, resistanceMagical,
            speed, attackSpeed, spellSpeed,
            regenerationMana, regenerationStamina, resistanceStatus,
            spread, range, dodgeRange,
            visualRange, critChance, critMultiplier
    };
    constexpr static auto subAttrByAttr = [] {
        std::array<std::array<SubAttributeName, 3>, 6> subAttrByAttr;
        subAttrByAttr[strength] = {damagePhysical, health, resistancePhysical};
        subAttrByAttr[intelligence] = {damageMagical, mana, resistanceMagical};
        subAttrByAttr[agility] = {speed, attackSpeed, spellSpeed};
        subAttrByAttr[willpower] = {regenerationMana, regenerationStamina, resistanceStatus};
        subAttrByAttr[coordination] = {spread, range, dodgeRange};
        subAttrByAttr[perception] = {visualRange, critChance, critMultiplier};
        return subAttrByAttr;

    }();
    constexpr static auto attrBySubAttr = [] {
        std::array<AttributeName, 18> attrBySubAttr;
        for (auto attr: attributeVec)
            for (auto subattr: subAttrByAttr[attr])
                attrBySubAttr[subattr] = attr;
        return attrBySubAttr;
    }();
    constexpr static std::array<char const *, 6> attributeString = {"strength",
                                                                    "intelligence",
                                                                    "agility",
                                                                    "willpower",
                                                                    "coordination",
                                                                    "perception",
    };
    constexpr static std::array<char const *, 18> subAttributeString = {"damagePhysical",
                                                                        "health",
                                                                        "resistancePhysical",
                                                                        "damageMagical",
                                                                        "mana",
                                                                        "resistanceMagical",
                                                                        "speed",
                                                                        "attackSpeed",
                                                                        "spellSpeed",
                                                                        "regenerationMana",
                                                                        "regenerationStamina",
                                                                        "resistanceStatus",
                                                                        "spread",
                                                                        "range",
                                                                        "dodgeRange",
                                                                        "visualRange",
                                                                        "critChance",
                                                                        "critMultiplier"};
    constexpr static std::array<int, 18> subAttrAtStart = {100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
                                                           100,
    };

}
using namespace AttributeConstants;


class Attributes {
public:


private:

    int level{1};


    std::array<int, 6> values{};
    std::array<int, 18> subValues{};
    std::array<float, 18> subValuesMultiplied{};
    std::vector<Modifier> attributeModifiers{};

    void Initialize() {

    }

public:

    Attributes(int st, int in, int ag, int wi, int co, int pe) {
        Initialize();
        values[strength] = st;
        values[intelligence] = in;
        values[agility] = ag;
        values[willpower] = wi;
        values[coordination] = co;
        values[perception] = pe;


        for (auto attr: attributeVec)
            for (auto subattr: subAttrByAttr[attr])
                subValues[subattr] = pointsAtStart;
    }

    Attributes() : Attributes(1, 1, 1, 1, 1, 1) {}

    void updateModifiers(const std::vector<Modifier> &modifiers) {
        attributeModifiers = modifiers;
    }

    [[nodiscard]] int getLevel() const { return level; }

    [[nodiscard]] int get(const AttributeName attr) const {
        int value = values[attr];
        for (auto modifier: attributeModifiers) {
            if (modifier.name == attr) {
                switch (modifier.operation) {
                    case ModifierOperator::ADD:
                        value += modifier.value;
                        break;
                    case ModifierOperator::MUL:
                        value *= modifier.value;
                        break;
                }
            }
        }
        return value;
    }

    [[nodiscard]] int get(const SubAttributeName subattr) const { return subValues[subattr]; }


    [[nodiscard]] float const *getPointerMultiplied(SubAttributeName subattr) {
        // TODO account for modifiers here
        subValuesMultiplied[subattr] = subValues[subattr] *
                                       config::attrMultipliers[subattr]; //TODO add a check that the attr is not too small to support the subattrs
        return &subValuesMultiplied[subattr];
    }

    [[nodiscard]] float getMultiplied(const SubAttributeName subattr) {
        //TODO add a check that the attr is not too small to support the subattrs
//        return subValuesMultiplied[subattr]; ?
        return subValues[subattr] * config::attrMultipliers[subattr];
    }

    int freeAttrPoints() const {
        return (level - 1) * pointsByLevel - std::accumulate(values.begin(), values.end(), 0) + 6;
    }

    int freeSubAttrPoints(AttributeName attr) const {
        int attrValue = get(attr);
        int subAttrTotal = 0;
        for (auto sa: subAttrByAttr.at(attr)) subAttrTotal += subValues[sa] - pointsAtStart;
        return pointsByAttr * attrValue - subAttrTotal;
    }

    int freeSubAttrPoints(SubAttributeName subattr) const {
        AttributeName attr = attrBySubAttr.at(subattr);
        return freeSubAttrPoints(attr);
    }

    bool outOfAttrPoints() const {
        return freeAttrPoints() <= 0;
    }

    bool outOfSubAttrPoints(const SubAttributeName subattr) const {
        return freeSubAttrPoints(subattr) <= 0;
    }

    bool atMinAttrPoints(AttributeName attr) const {
        int subAttrTotal = 0;
        for (auto sa: subAttrByAttr.at(attr)) subAttrTotal += subValues[sa] - pointsAtStart;
        return values[attr] <= 1 || subAttrTotal > pointsByAttr * values[attr] - pointsByAttr;
    }


    bool atMinSubAttrPoints(SubAttributeName subattr) const {
        return subValues[subattr] <= 0;
    }


    void decrease(const AttributeName attr) {
        if (atMinAttrPoints(attr)) return;
        values[attr] -= 1;
    }

    void decrease(const SubAttributeName subattr) {
        if (atMinSubAttrPoints(subattr)) return;
        subValues[subattr] -= 1;
    }

    void increase(const AttributeName attr) {
        if (outOfAttrPoints()) {
            std::cout << "You don't have free attribute points\n";
            return;
        }
        values[attr] += 1;
    }

    void increase(const SubAttributeName subattr) {
        if (outOfSubAttrPoints(subattr)) {
            std::cout << "You don't have free subattribute points\n";
            return;
        }
        subValues[subattr] += 1;
    }


    int expForCurrentLevel() const {
        int total = 0;
        int l = level - 1;
        while (l > 0) {
            total += l * expByLevel;
            l -= 1;
        }
        return total;
    }

    int expToNextLevel() const {
        return expForCurrentLevel() + level * expByLevel;
    }

    void levelUp() {
        level += 1;
    }

};


#endif //APTOMODO_ATTRIBUTES_H
