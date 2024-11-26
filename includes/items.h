//
// Created by ginko on 07/05/24.
//

#ifndef APTOMODO_ITEMS_H
#define APTOMODO_ITEMS_H

/*
    Detect items when close (maybe in the collision loop)
     show a toooltip
     every item could store a map <subattr, float> to be checked when calculating final subattr values
     it could work like this: in updatePlayer we calculate the subattrs using
        1) The attributes themselves with the function getMultiplied
        2) A view of all the buffs on the player (registry.view<Buff, OnPlayer, Attributes::Modifier)
        3) A view of all the items on the player (registry.view<Item, OnPlayer, Attributes::Modifier)
    The action of picking up an item will mean
        1) Remove its component Position
        2) Add tag OnPlayer
    The Items will be then just an entity created through a factory.
    The factory will take in the enum of the item and create it (with randomness if wanted)
    To assign them spawn probabilities we will need some table (or a separated registry) where we can sort by rarity
 function l
    We will need aoadItemFromFile
 */

struct OnPlayer {};
struct Item {};
struct Name {
    std::string value;
    operator std::string() const noexcept { return value; }
};
// TODO save items in files
// TODO add an item editor

//
//struct SubAttributeModifier {
//    std::map<Attributes::SubAttributeName, float> dict;
//};
//


#endif //APTOMODO_ITEMS_H
