//
// Created by ginko on 11/24/24.
//

#ifndef ENEMYTYPE_H
#define ENEMYTYPE_H


struct EnemyType {
    std::string name;
    int grade;
    float radius;
    float speed;
    float attackSpeed;
    float damage;
    float attackRange;
    float attackSpread;
    float health;
    int experience;
    std::string attributesPath;
    std::string texturePath;
};

inline constexpr std::string_view root_path = ROOT_PATH;
inline constexpr std::string_view config_path = CONFIG_PATH;


struct EnemyDataFile {
    const static std::filesystem::path path;
    std::vector<EnemyType> enemyStats;
    static constexpr std::array headers{
            "Name", "Grade", "Radius", "Speed", "Attack Speed", "Damage",
            "Attack Range", "Attack Spread", "Health", "Experience",
            "Attributes Path", "Texture Path"
    };


    bool loadCSV();

    EnemyType &getType(const std::string &);

    static bool saveCSV(const std::vector<EnemyType>& enemies);

};

#endif //ENEMYTYPE_H
