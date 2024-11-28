#include "includes/constants.h"
#include "includes/config.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/parameters.h"
#include "managers/assets.h"
#include "managers/game.h"


int main() {
    InitWindow(Const::screenWidth, Const::screenHeight, "Apto Modo");
    ToggleFullscreen();
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    Params::Instantiate();

    Game::Loop();
    CloseWindow(); // ~Game
    return 0;
}
