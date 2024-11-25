//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_AUDIOMANAGER_H
#define APTOMODO_AUDIOMANAGER_H

// TODO Remove all includes

#include <raylib.h>
#include <unordered_map>
#include <string>
#include "../constants.h"

/*
 * TODO Put all the assets in an AssetManager and leave only a namespace with function Instantiate and Playe
 */
class AudioManager {
    static std::unordered_map<size_t, Sound> resources;
    static std::hash<std::string> hasher;
    static AudioManager *instance;

public:
    Sound &operator[](std::string &key) const;

    static void LoadFromFile(const std::string &filename);

    static AudioManager &Instance();

    static void Instantiate() { // TODO Instantiate doesn't need to be in a class
        instance = new AudioManager();
        InitAudioDevice();
        SetMasterVolume(0);
        LoadFromFile("player_shot"); // TODO load these into an AssetManager
        LoadFromFile("enemy_shot");
        LoadFromFile("enemy_explosion");
    }

    void Play(const std::string &key) const;

//    ~AudioManager destroy all files
};


#endif //APTOMODO_AUDIOMANAGER_H
