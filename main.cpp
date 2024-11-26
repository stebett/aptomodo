#include "includes/constants.h"
#include "includes/config.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/parameters.h"
#include "managers/assets.h"
#include "managers/game.h"

/* TODO All the level related stuff and the game loop function should be in another file
 */


int main() {
    InitWindow(screenWidth, screenHeight, "Apto Modo"); // TODO WindowManager::Instantiate();
    ToggleFullscreen(); // This will be inside WindowManager::Instantiate()
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    Params::Instantiate();

    Game::Loop();
    CloseWindow(); // ~Game
    return 0;
}
