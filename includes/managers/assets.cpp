//
// Created by ginko on 11/26/24.
//

#include "assets.h"
#include <constants.h>

#include "levelManager.h"

std::unordered_map<size_t, Sound> Assets::audioResources;
std::hash<std::string> Assets::hasher;

EnemyDataFile Assets::enemiesData;

std::unique_ptr<ldtk::Project> Assets::LDTKProject;


void Assets::LoadAudio(const std::string &filename) {
    audioResources.insert({hasher(filename), LoadSound(getAssetPath("tracce/" + filename + ".mp3").c_str())});
}

const Sound &Assets::GetSound(const std::string &name) { return audioResources.at(hasher(name)); }


const ldtk::Level &Assets::GetLevel(const int levelNumber) {
    assert(LDTKProject && "ERROR ASSETS LDTK Project has already been cleared");
    return LDTKProject->getWorld().getLevel(std::format("Level_{}", levelNumber));;
}

EnemyDataFile &Assets::GetEnemiesData() {
    return enemiesData;
}

void Assets::CleanAudio() {
    for (auto & audioResource : audioResources) {
        SPDLOG_INFO("[ASSETS] Unloading sound"); // maybe for debugging find a way to get the name
        UnloadSound(audioResource.second);
    }
}

void Assets::CleanLDTK() {
    LDTKProject.reset();

}

void Assets::Clean() {
    CleanLDTK();
    CleanAudio();
}

void Assets::InstantiateAudio() {
    InitAudioDevice();
    SetMasterVolume(0);
    LoadAudio("player_shot"); // TODO get all names from a file, or load all files in a directory
    LoadAudio("enemy_shot");
    LoadAudio("enemy_explosion");
}

void Assets::InstantiateEnemiesFile() {
    enemiesData.loadCSV(enemiesData.path);
}


void Assets::InstantiateLevel() {
    LDTKProject = std::make_unique<ldtk::Project>();
    SPDLOG_DEBUG("[ASSETS] LDTK Project path: {}", getAssetPath("project.ldtk"));
    LDTKProject->loadFromFile(getAssetPath("project.ldtk"));

}

void Assets::Instantiate() {
    InstantiateAudio();
    InstantiateLevel();
    InstantiateEnemiesFile();
}
