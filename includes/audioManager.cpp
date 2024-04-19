//
// Created by ginko on 17/04/24.
//

#include "audioManager.h"

std::unordered_map<size_t, Sound> AudioManager::resources;
std::hash<std::string> AudioManager::hasher;
const char *AudioManager::root = "/home/ginko/Documents/tracce/";
AudioManager AudioManager::instance;

Sound &AudioManager::operator[](std::string &key) const {
    return resources.at(hasher(key));
}

void AudioManager::LoadFromFile(const std::string &filename) {
    resources.insert({hasher(filename), LoadSound((root + filename + ".mp3").c_str())});
}

// TODO make a copy for each sound?
void AudioManager::Play(const std::string &key) const {
    PlaySound(resources.at(hasher(key)));
}

AudioManager &AudioManager::Instance() { return instance; }

// TODO this needs to be attached to an event system: everytime the player attacks this has to trigger updateAudio and updateAnimation and so on
//void updateAudio(entt::registry &registry) {
//    playerView = registry.view<Player,
//}