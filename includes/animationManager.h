//
// Created by ginko on 19/04/24.
//

#ifndef ACEROLA_JAM0_ANIMATIONMANAGER_H
#define ACEROLA_JAM0_ANIMATIONMANAGER_H

#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>

struct TextureAnimation {
    std::vector <Texture2D> textures {};
    unsigned int frameCount {0};
};

class AnimationManager {
    static std::unordered_map <size_t, TextureAnimation> resources;
    static std::hash <std::string> hasher;
    static const char *root;
    static AnimationManager instance;

public:
    static Texture2D& getTexture(std::string &key, unsigned int frame) ;

    static void LoadFromDirectory(const std::string &directory);

    static AnimationManager &Instance();

    static void Instantiate() {
        instance = *new AnimationManager();
        LoadFromDirectory("enemy/walking/v1/");
    }
};




#endif //ACEROLA_JAM0_ANIMATIONMANAGER_H
