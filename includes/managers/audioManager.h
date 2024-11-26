//
// Created by ginko on 17/04/24.
//

#ifndef APTOMODO_AUDIOMANAGER_H
#define APTOMODO_AUDIOMANAGER_H


namespace Audio {
    struct Command {
        std::string soundName;
    };

     void Play(const std::string &key);
};


#endif //APTOMODO_AUDIOMANAGER_H
