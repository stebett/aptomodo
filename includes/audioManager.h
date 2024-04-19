//
// Created by ginko on 17/04/24.
//

#ifndef ACEROLA_JAM0_AUDIOMANAGER_H
#define ACEROLA_JAM0_AUDIOMANAGER_H

#include <raylib.h>
#include <unordered_map>
#include <string>

class AudioManager {
    static std::unordered_map<size_t, Sound> resources;
    static std::hash<std::string> hasher;
    static const char *root;
    static AudioManager instance;

public:
    Sound& operator[](std::string &key) const;

    static void LoadFromFile(const std::string &filename);

    static AudioManager& Instance();

    static void Instantiate() {
        instance = *new AudioManager();
        InitAudioDevice();      // Initialize audio device
        LoadFromFile("player_shot");
        LoadFromFile("enemy_shot");
        LoadFromFile("enemy_explosion");
    }

    void Play(const std::string& key) const;

//    ~AudioManager destroy all files
};


#endif //ACEROLA_JAM0_AUDIOMANAGER_H
