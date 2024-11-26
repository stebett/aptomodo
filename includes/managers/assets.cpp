//
// Created by ginko on 11/26/24.
//

#include "assets.h"

#include <constants.h>

std::unordered_map<size_t, Sound> Assets::audioResources;
std::hash<std::string> Assets::hasher;

void Assets::LoadAudio(const std::string &filename) {
    audioResources.insert({hasher(filename), LoadSound(getAssetPath("tracce/" + filename + ".mp3").c_str())});
}

const Sound &Assets::GetSound(const std::string &name) { return audioResources.at(hasher(name)); }

void Assets::InstantiateAudio() {
    InitAudioDevice();
    // SetMasterVolume(0);
    LoadAudio("player_shot"); // TODO get all names from a file, or load all files in a directory
    LoadAudio("enemy_shot");
    LoadAudio("enemy_explosion");
}
void Assets::Instantiate() {
    InstantiateAudio();
}
