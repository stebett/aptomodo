//
// Created by ginko on 16/04/24.
//
#pragma once

#include <array>

namespace config {
    extern bool show_astar_path;
    extern bool show_enemy_fov;
    extern bool show_bounding_box;
    extern int fps;
    extern int strategy;
    extern int enemy_walking_animation_fps;
    extern bool show_enemy_texture;
    extern bool show_attacks;
    extern bool show_attr_window;
    extern std::array<float, 18> attrMultipliers;
}