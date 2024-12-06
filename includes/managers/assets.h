//
// Created by ginko on 11/26/24.
//

#ifndef ASSETS_H
#define ASSETS_H

#include "resources/enemyType.h"
#include "intGrid.h"
#include "resources/audioAssets.h"


class Assets {

    static std::unique_ptr<ldtk::Project> LDTKProject;

    static EnemyDataFile enemiesData;

    static void InstantiateAudio();

    static void InstantiateLevel();

public:
    static std::unordered_map<std::size_t, SoundEffect> audioResources;

    static void LoadAudio(const std::string &filename);

    static const Sound &GetSound(const std::string &name);

    static const Texture2D &GetTexture();

    static const ldtk::Level &GetLevel(int levelNumber);

    static EnemyDataFile &GetEnemiesData();

    static IntGrid &GetGrid();

    static void Instantiate();

    static void Clean();

    static void CleanAudio();

    static void CleanLDTK();

    static void InstantiateEnemiesFile();

};

#endif //ASSETS_H
