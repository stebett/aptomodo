//
// Created by ginko on 11/26/24.
//

#ifndef GAME_H
#define GAME_H
#include "intGrid.h"

enum class LevelOutcome {
    WIN,
    LOSE,
    RESTART,
    QUIT,
    NONE,
};


class Game {
private:
    static bool paused;;
public:
    int Level{};
    static Texture2D levelTexture;
    static IntGrid grid;


    static void Loop();
    static bool IsPaused();
    static void Pause();
};


#endif //GAME_H
