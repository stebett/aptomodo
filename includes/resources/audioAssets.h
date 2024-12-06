//
// Created by ginko on 12/6/24.
//

#ifndef APTOMODO_AUDIOASSETS_H
#define APTOMODO_AUDIOASSETS_H

struct SoundEffect {
    std::size_t id;
    std::filesystem::path path;
    std::string name;
    Sound sound;
};

namespace Audio {
    std::vector<std::string> loadCSV();

    bool saveCSV(const std::vector<std::string> &filenames);
}


#endif //APTOMODO_AUDIOASSETS_H
