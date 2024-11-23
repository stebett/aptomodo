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
    extern bool show_inv_window;
    extern bool free_camera;
    extern bool draw_level_collisions;
    extern bool show_enemy_window;
    extern std::array<float, 18> attrMultipliers;
    extern float enemySightRange;
    extern float enemySightRangeChasing;
    extern float enemyHearRange;
    extern float enemyHearRangeChasing;
    extern float enemyPatrolDistance;
}


constexpr char const * configPath{"config.toml"};

class Config {
    static toml::table config;

public:
    toml::table writeTable();


    static void SaveAttributeParameters();
    static void LoadAttributeParameters();

    static int GetInt(const std::string& name);
    static float GetFloat(const std::string& name);
    static bool GetBool(const std::string& name);


    static void Instantiate() {
        LoadAttributeParameters();
    }


    //    ~Parameters destroy unloads all
};
