//
// Created by ginko on 16/04/24.
//

#include "config.h"
#include "attributes.h"

namespace config {
    bool show_astar_path = false;
    bool show_enemy_fov = false;
    bool show_bounding_box = true;
    bool show_enemy_texture = false;
    bool show_attacks = true;
    bool show_attr_window = false;
    bool show_inv_window = false;
    bool free_camera = false;
    bool draw_level_collisions = false;

    int fps = 60;
    int enemy_walking_animation_fps = 8;
    int strategy = 0;


    std::array<float, 18> attrMultipliers{
            3.0f, // damagePhysical
            10.0f, // health
            10.0f, // resistancePhysical
            10.0f, // damageMagical
            10.0f, // mana
            10.0f, // resistanceMagical
            0.50f, // speed
            0.03f, // attackSpeed
            10.0f, // spellSpeed
            10.0f, // regenerationMana
            10.0f, // regenerationStamina
            10.0f, // resistanceStatus
            1.0f, // spread
            6.0f, // range
            10.0f, // dodgeRange
            10.0f, // visualRange
            10.0f, // critChance
            10.0f, // critMultiplier
    };
};