//
// Created by ginko on 11/26/24.
//

#include "assets.h"
#include <constants.h>

#include "levelManager.h"

std::unordered_map<std::size_t, SoundEffect> Assets::audioResources;

EnemyDataFile Assets::enemiesData;

std::unique_ptr<ldtk::Project> Assets::LDTKProject;


void Assets::LoadAudio(const std::string &filename) {
    auto id{entt::hashed_string(filename.data())};
    auto path{std::filesystem::path(ROOT_PATH) / std::filesystem::path(AUDIO_PATH) / filename };
    const auto& name{filename};
    auto sound{LoadSound(path.replace_extension(".mp3").c_str())};
    audioResources.insert({id, SoundEffect(id, path, name, sound)});
}

const Sound &Assets::GetSound(const std::string &name) {
    return audioResources.at(entt::hashed_string(name.data())).sound;
}


const ldtk::Level &Assets::GetLevel(const int levelNumber) {
    assert(LDTKProject && "ERROR ASSETS LDTK Project has already been cleared");
    return LDTKProject->getWorld().getLevel(std::format("Level_{}", levelNumber));;
}

EnemyDataFile &Assets::GetEnemiesData() {
    return enemiesData;
}

void Assets::CleanAudio() {
    for (auto &audioResource: audioResources) {
        SPDLOG_INFO("[ASSETS] Unloading sound"); // maybe for debugging find a way to get the name
        UnloadSound(audioResource.second.sound);
    }
    CloseAudioDevice();
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
    for (const auto& name: Audio::loadCSV())
        LoadAudio(name);
}

void Assets::InstantiateEnemiesFile() {
    enemiesData.loadCSV();
}


void Assets::InstantiateLevel() {
    LDTKProject = std::make_unique<ldtk::Project>();
    SPDLOG_DEBUG("[ASSETS] Loading LDTK Project");
    SPDLOG_DEBUG("[ASSETS] Loading LDTK Project");
    LDTKProject->loadFromFile(getAssetPath("project.ldtk"));

}

void Assets::Instantiate() {
    InstantiateAudio();
    InstantiateLevel();
    InstantiateEnemiesFile();
}
