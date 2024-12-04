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
    Color color;
    float health;
    int experience;
    std::string attributesPath;
    std::string texturePath;
};

struct EnemyDataFile {
    std::string path{"config/enemies.csv"};
    std::vector<std::string> headers;
    std::vector<EnemyType> enemyStats;

    bool loadCSV(const std::string &filename);
    EnemyType &getType(const std::string&);
};

#endif //ENEMYTYPE_H
