//
// Created by ginko on 11/26/24.
//

#ifndef ASSETS_H
#define ASSETS_H



class Assets {
    static std::unordered_map<size_t, Sound> audioResources;
    static std::hash<std::string> hasher;
    static void InstantiateAudio();

public:
    static void LoadAudio(const std::string &filename);
    static const Sound& GetSound(const std::string &name);


    static void Instantiate();

};



#endif //ASSETS_H
