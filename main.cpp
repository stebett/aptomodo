#include "includes/config.h"
#include "includes/constants.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/assets.h"
#include "includes/managers/game.h"
#include "includes/managers/logging.h"
#include "includes/managers/parameters.h"


int main() {
    Log::Instantiate();
    InitWindow(Const::screenWidth, Const::screenHeight, "Apto Modo");
    ToggleFullscreen();
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    Params::Instantiate();

    Game::Level = 2;

    Game::Loop();
    CloseWindow();
    return 0;
}
