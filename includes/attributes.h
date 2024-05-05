//
// Created by ginko on 04/05/24.
//

#ifndef ACEROLA_JAM0_ATTRIBUTES_H
#define ACEROLA_JAM0_ATTRIBUTES_H

#include <array>
#include <unordered_map>
#include <numeric>
#include <iostream>



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
public:
    constexpr static std::array<AttributeName, 6> attributeVec{strength, intelligence, agility, willpower, coordination,
                                                               perception,};

    std::unordered_map<AttributeName, std::array<SubAttributeName, 3>> subAttrByAttr;  // TODO make constexpr static
    std::unordered_map<SubAttributeName, AttributeName> attrBySubAttr;
    std::unordered_map<AttributeName, std::string> attributeString;
private:

    int level;
    static constexpr int pointsByLevel{3};
    static constexpr int pointsByAttr{3};

    std::array<int, 6> values{};
    std::array<int, 18> subValues{};


    void Initialize() {
        subAttrByAttr[strength] = {damagePhysical, health, resistancePhysical};
        subAttrByAttr[intelligence] = {damageMagical, mana, resistanceMagical};
        subAttrByAttr[agility] = {speed, attackSpeed, spellSpeed};
        subAttrByAttr[willpower] = {regenerationMana, regenerationStamina, resistanceStatus};
        subAttrByAttr[coordination] = {spread, range, dodgeRange};
        subAttrByAttr[perception] = {visualRange, critChance, critMultiplier};
        for (auto attr: attributeVec)
            for (auto subAttr: subAttrByAttr[attr])
                attrBySubAttr[subAttr] = attr;
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
            for (auto subAttr: subAttrByAttr[attr])
                subValues[subAttr] = values[attr];
    }

    Attributes() {
        Attributes(1, 1, 1, 1, 1, 1);
    }

    [[nodiscard]] int get(AttributeName attr) const { return values[attr]; }

    [[nodiscard]] int get(SubAttributeName subAttr) const { return subValues[subAttr]; }

    void increase(AttributeName attr) {
        if (std::accumulate(values.begin(), values.end(), -6 + pointsByLevel) == level * pointsByLevel) {
            std::cout << "You don't have free attribute points\n";
            return;
        }
        values[attr] += 1;
    }

    void increase(SubAttributeName subAttr) {
        AttributeName attr = attrBySubAttr[subAttr];
        int attrValue = values[attr];
        int subAttrTotal = 0;
        for (auto sa: subAttrByAttr[attr]) subAttrTotal += subValues[subAttr];
        if (subAttrTotal > pointsByAttr * attrValue) {
            std::cout << "You don't have free subattribute points\n";
            return;
        }
        subValues[subAttr] += 1;
    }

    void decrease(SubAttributeName subAttr) {
        if (subValues[subAttr] <= 0) return;
        subValues[subAttr] -= 1;
    }


//    bool maxed() const {
//        return
//                strength.value + intelligence.value + agility.value + willpower.value + coordination.value +
//                perception.value >= level;
//    }
};

#endif //ACEROLA_JAM0_ATTRIBUTES_H
