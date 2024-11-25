//
// Created by ginko on 21/05/24.
//

#ifndef CONFIGURATION_ATTRIBUTESCONSTANTS_H
#define CONFIGURATION_ATTRIBUTESCONSTANTS_H


namespace AttributeConstants {
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


    enum AttributeName {
        strength,
        intelligence,
        agility,
        willpower,
        coordination,
        perception,
    };

    constexpr static std::array<char const *, 6> attributeString = {"strength",
                                                                    "intelligence",
                                                                    "agility",
                                                                    "willpower",
                                                                    "coordination",
                                                                    "perception",
    };
    enum class ModifierOperator {
        ADD,
        MUL
    };

    constexpr std::array<char const *, 2> operatorString{
            "ADD",
            "MUL"
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
        std::array<std::array<SubAttributeName, 3>, 6> subAttrByAttr; // TODO init
        subAttrByAttr[strength] = {damagePhysical, health, resistancePhysical};
        subAttrByAttr[intelligence] = {damageMagical, mana, resistanceMagical};
        subAttrByAttr[agility] = {speed, attackSpeed, spellSpeed};
        subAttrByAttr[willpower] = {regenerationMana, regenerationStamina, resistanceStatus};
        subAttrByAttr[coordination] = {spread, range, dodgeRange};
        subAttrByAttr[perception] = {visualRange, critChance, critMultiplier};
        return subAttrByAttr;

    }();
    constexpr static auto attrBySubAttr = [] {
        std::array<AttributeName, 18> attrBySubAttr;// TODO init
        for (auto attr: attributeVec)
            for (auto subattr: subAttrByAttr[attr])
                attrBySubAttr[subattr] = attr;
        return attrBySubAttr;
    }();

}
#endif //CONFIGURATION_ATTRIBUTESCONSTANTS_H
