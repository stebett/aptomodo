//
// Created by ginko on 11/26/24.
//

#ifndef GAME_H
#define GAME_H
#include <camera.h>
#include "framerateManager.h"
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
    static entt::registry registry;

    static GameCamera camera;
    static FramerateManager framerateManager;

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

    static void LoopOnce();

    static void CleanLevel();

    static void PrepareLevel(const int levelNumber);

    static LevelOutcome PlayLevelOnce();
};


#endif //GAME_H
