//
// Created by ginko on 04/05/24.
//

#ifndef ACEROLA_JAM0_ATTRIBUTES_H
#define ACEROLA_JAM0_ATTRIBUTES_H

#include <array>
#include <unordered_map>
#include <numeric>
#include <iostream>
#include "config.h"


//struct Attribute {
//    int value;
//    std::map<AttributeName, int> subattributes;
//
//    static int pointsAtStart() { return 1; }
//
//    [[nodiscard]] int pointsByLevel() const { return value * 2; }
//
//    [[nodiscard]] bool maxed() const {
//        return std::accumulate(subattributes.begin(), subattributes.end(), pointsAtStart(),
//                               [](const std::size_t previous, const auto &element) {
//                                   return previous + element.second;
//                               }) >= pointsByLevel();
//    }
//};

class Attributes {
public:
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
    constexpr static std::array<AttributeName, 6> attributeVec{strength, intelligence, agility, willpower, coordination,
                                                               perception,};

    constexpr static std::array<SubAttributeName, 18> subAttributeVec{
            damagePhysical, health, resistancePhysical,
            damageMagical, mana, resistanceMagical,
            speed, attackSpeed, spellSpeed,
            regenerationMana, regenerationStamina, resistanceStatus,
            spread, range, dodgeRange,
            visualRange, critChance, critMultiplier
    };

    std::unordered_map<AttributeName, std::array<SubAttributeName, 3>> subAttrByAttr;  // TODO make constexpr static
    std::unordered_map<SubAttributeName, AttributeName> attrBySubAttr;
    std::unordered_map<AttributeName, char const *> attributeString;
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

private:

    int level{1};
    static constexpr int expByLevel{100};
    static constexpr int pointsByLevel{3};
    static constexpr int pointsByAttr{3};
    static constexpr int pointsAtStart{10};

    std::array<int, 6> values{};
    std::array<int, 18> subValues{};
    std::array<float, 18> subValuesMultiplied{};


    void Initialize() {
        subAttrByAttr[strength] = {damagePhysical, health, resistancePhysical};
        subAttrByAttr[intelligence] = {damageMagical, mana, resistanceMagical};
        subAttrByAttr[agility] = {speed, attackSpeed, spellSpeed};
        subAttrByAttr[willpower] = {regenerationMana, regenerationStamina, resistanceStatus};
        subAttrByAttr[coordination] = {spread, range, dodgeRange};
        subAttrByAttr[perception] = {visualRange, critChance, critMultiplier};
        for (auto attr: attributeVec)
            for (auto subattr: subAttrByAttr[attr])
                attrBySubAttr[subattr] = attr;
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

        attributeString[strength] = "strength";
        attributeString[intelligence] = "intelligence";
        attributeString[agility] = "agility";
        attributeString[willpower] = "willpower";
        attributeString[coordination] = "coordination";
        attributeString[perception] = "perception";


        for (auto attr: attributeVec)
            for (auto subattr: subAttrByAttr[attr])
                subValues[subattr] = pointsAtStart;
    }

    Attributes() : Attributes(1, 1, 1, 1, 1, 1) {}


    [[nodiscard]] int getLevel() const { return level; }

    [[nodiscard]] int get(const AttributeName attr) const { return values[attr]; }

    [[nodiscard]] int get(const SubAttributeName subattr) const { return subValues[subattr]; }

    [[nodiscard]] float const *getPointerMultiplied(SubAttributeName subattr) {
        subValuesMultiplied[subattr] = subValues[subattr] * config::attrMultipliers[subattr];
        return &subValuesMultiplied[subattr];
    }

    [[nodiscard]] float getMultiplied(const SubAttributeName subattr) {
        return subValues[subattr] * config::attrMultipliers[subattr];
    }


    bool outOfAttrPoints() const {
        return std::accumulate(values.begin(), values.end(), 0) >= level * pointsByLevel;
    }

    bool outOfSubAttrPoints(const SubAttributeName subattr) const {
        AttributeName attr = attrBySubAttr.at(subattr);
        int attrValue = values[attr];
        int subAttrTotal = 0;
        for (auto sa: subAttrByAttr.at(attr)) subAttrTotal += subValues[sa] - pointsAtStart;
        return subAttrTotal >= pointsByAttr * attrValue;
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

    int getExpByLevel() const {
        return level * expByLevel;
    }

    void levelUp() {
        level += 1;
    }

};

#endif //ACEROLA_JAM0_ATTRIBUTES_H
