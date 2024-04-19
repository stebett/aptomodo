//
// Created by ginko on 19/04/24.
//

#include "animationManager.h"
#include <filesystem>
#include <set>
namespace fs = std::filesystem;

std::unordered_map<size_t, TextureAnimation> AnimationManager::resources;
std::hash<std::string> AnimationManager::hasher;
const char *AnimationManager::root = "/home/ginko/jems-project/acerola-jam0/assets/";
AnimationManager AnimationManager::instance;

Texture2D &AnimationManager::getTexture(const std::string &key, unsigned int frame) {
    TextureAnimation &anim = resources.at(hasher(key));
    return anim.textures[frame % anim.frameCount];
}

void AnimationManager::LoadFromDirectory(const std::string &directory) {
    TextureAnimation anim;
    std::set<fs::path> sorted_by_name;

    for (auto &entry : fs::directory_iterator(root + std::string(directory))) {
        sorted_by_name.insert(entry.path());
    }

    for (auto &filename : sorted_by_name) {
        anim.textures.push_back(LoadTexture(filename.c_str()));
        anim.frameCount++;
    }

    resources.insert({hasher(directory), anim});
}


AnimationManager &AnimationManager::Instance() { return instance; }

