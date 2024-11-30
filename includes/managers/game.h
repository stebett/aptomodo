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
    static bool levelFinished;
    static LevelOutcome levelOutcome;

public:
    static int Level;
    static Texture2D levelTexture;
    static IntGrid grid;


    static void Loop();

    static bool IsPaused();

    static void Pause();

    static void ExitLevel();

    static void EnterLevel();

    static void SetOutcome(LevelOutcome);

    [[nodiscard]] static bool IsLevelFinished();

    [[nodiscard]] static LevelOutcome GetOutcome();
};


#endif //GAME_H
